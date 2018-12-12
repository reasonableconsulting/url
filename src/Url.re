type t = {
  href: string,
  protocol: option(string),
  slashes: bool,
  origin: option(string),
  host: option(string),
  hostname: option(string),
  port: option(int),
  auth: option(string),
  username: option(string),
  password: option(string),
  pathname: option(string),
  querystring: option(string),
  /* query: option(Query.t), */
  hash: option(string),
};

module Option = {
  let isNone = opt =>
    switch (opt) {
    | None => true
    | Some(_) => false
    };

  let getWithDefault = (opt, default) =>
    switch (opt) {
    | Some(value) => value
    | None => default
    };

  let mapWithDefault = (opt, default, fn) =>
    switch (opt) {
    | Some(value) => fn(value)
    | None => default
    };

  let map = (opt, fn) =>
    switch (opt) {
    | Some(value) => Some(fn(value))
    | None => None
    };
};

module Str = {
  let lowercase = str => String.lowercase(str);

  let index = (str, chr) =>
    switch (String.index(str, chr)) {
    | exception Not_found => None
    | index => Some(index)
    };

  let slice = (str, start, len) =>
    switch (String.sub(str, start, len)) {
    | exception (Invalid_argument(_)) => None
    | sub => Some(sub)
    };

  let length = str =>
    switch (String.length(str)) {
    | exception (Invalid_argument(_)) => None
    | len => Some(len)
    };

  let startsWith = (str, chr) =>
    switch (String.index(str, chr)) {
    | exception Not_found => false
    | index => index === 0
    };

  let endsWith = (str, chr) =>
    switch (String.rindex(str, chr), String.length(str)) {
    | exception Not_found => false
    | exception (Invalid_argument(_)) => false
    | (index, length) => index === length - 1
    };

  let sliceToEnd = (str, start) =>
    switch (length(str)) {
    | Some(length) => slice(str, start, length - start)
    | None => None
    };

  let split = (str, chr) => {
    switch (index(str, chr)) {
    | Some(index) =>
      let beginChunk = slice(str, 0, index);
      let endChunk = sliceToEnd(str, index + 1);
      [|beginChunk, endChunk|];
    | None => [|Some(str)|]
    };
  };

  let chunkBegin = (str, index) => {
    let rest = sliceToEnd(str, index + 1);
    let beginChunk = slice(str, 0, index);

    switch (rest) {
    | Some(rest) => (beginChunk, rest)
    /* TODO: Could the rest ever fail but the beginChunk succeed? */
    | None => (None, str)
    };
  };

  let chunkEnd = (str, index) => {
    let rest = slice(str, 0, index);
    let endChunk = sliceToEnd(str, index);

    switch (rest) {
    | Some(rest) => (endChunk, rest)
    /* TODO: Could the rest ever fail but the endChunk succeed? */
    | None => (None, str)
    };
  };
};

/* TODO: How does these behave with a native compilation target? */
let protocolRegExp = [%bs.re {|/^([a-z][a-z0-9.+-]*:)?(\/\/)?([\S\s]*)/i|}];
let slashesRegExp = [%bs.re {|/^[A-Za-z][A-Za-z0-9+-.]*:\/\//|}];
let portRegExp = [%bs.re {|/:(\d+)$/|}];

module Rules = {
  let hash = address =>
    switch (Str.index(address, '#')) {
    | Some(index) => Str.chunkEnd(address, index)
    | None => (None, address)
    };

  let querystring = address =>
    switch (Str.index(address, '?')) {
    | Some(index) => Str.chunkEnd(address, index)
    | None => (None, address)
    };

  let sanitize = address =>
    String.map(char => char === '\\' ? '/' : char, address);

  let pathname = address => {
    let address = sanitize(address);

    switch (Str.index(address, '/')) {
    | Some(index) => Str.chunkEnd(address, index)
    | None => (None, address)
    };
  };

  let auth = address =>
    switch (Str.index(address, '@')) {
    | Some(index) => Str.chunkBegin(address, index)
    | None => (None, address)
    };

  let host = address =>
    /* TODO: add relative stuff? */
    switch (Str.length(address)) {
    | Some(_) => (Some(Str.lowercase(address)), address)
    | None => (None, address)
    };

  let port = address => {
    let match = Js.Re.exec(address, portRegExp);

    switch (match) {
    | Some(result) =>
      let captured = Js.Re.captures(result);

      let port =
        switch (Js.Nullable.toOption(captured[1])) {
        | Some(port) => Some(port)
        | None => None
        };

      switch (Str.slice(address, 0, Js.Re.index(result))) {
      | Some(address) => (port, address)
      | None => (port, "")
      };
    | None => (None, address)
    };
  };

  let hostname = address =>
    /* TODO: add relative stuff? */
    switch (Str.length(address)) {
    | Some(_) => (Some(Str.lowercase(address)), address)
    | None => (None, address)
    };

  let rec exec =
          (
            ~rules=[hash, querystring, pathname, auth, host, port, hostname],
            ~results=[],
            address,
          ) =>
    switch (rules) {
    | [rule, ...rules] =>
      let (result, address) = rule(address);
      let results = [result, ...results];
      exec(~rules, ~results, address);
    | [] => results
    };
};

let requiresPort = (port, protocol) => {
  switch (port, protocol) {
  | (Some(port), Some(protocol)) =>
    let protocol = Str.split(protocol, ':')[0];

    switch (protocol) {
    | Some("http")
    | Some("ws") => port !== 80
    | Some("https")
    | Some("wss") => port !== 443
    | Some("ftp") => port !== 21
    | Some("gopher") => port !== 70
    | Some("file") => false
    | _ => port !== 0
    };
  | _ => false
  };
};
let extractProtocol = address => {
  let match = Js.Re.exec(address, protocolRegExp);

  switch (match) {
  | Some(result) =>
    let captured = Js.Re.captures(result);

    let protocol =
      switch (Js.Nullable.toOption(captured[1])) {
      | Some(protocol) => Some(String.lowercase(protocol))
      | None => None
      };

    let slashes =
      switch (Js.Nullable.toOption(captured[2])) {
      | Some(_) => true
      | None => false
      };

    let rest = Js.Nullable.toOption(captured[3]);

    (protocol, slashes, rest);
  | None => (None, false, None)
  };
};

/* let resolve = (relative, base) => {
     var path = (base || '/').split('/').slice(0, -1).concat(relative.split('/'))
       , i = path.length
       , last = path[i - 1]
       , unshift = false
       , up = 0;

     while (i--) {
       if (path[i] === '.') {
         path.splice(i, 1);
       } else if (path[i] === '..') {
         path.splice(i, 1);
         up++;
       } else if (up) {
         if (i === 0) unshift = true;
         path.splice(i, 1);
         up--;
       }
     }

     if (unshift) path.unshift('');
     if (last === '.' || last === '..') path.push('');

     return path.join('/');
   } */

let toString = url => {
  Option.mapWithDefault(url.protocol, "", protocol =>
    Str.endsWith(protocol, ':') ? protocol : protocol ++ ":"
  )
  ++ (url.slashes ? "//" : "")
  ++ Option.mapWithDefault(url.username, "", username =>
       username
       ++ Option.mapWithDefault(url.password, "", password => ":" ++ password)
       ++ "@"
     )
  ++ Option.getWithDefault(url.host, "")
  ++ Option.getWithDefault(url.pathname, "")
  /* TODO: Handle query serialization */
  ++ Option.mapWithDefault(url.querystring, "", querystring =>
       Str.startsWith(querystring, '?') ? querystring : "?" ++ querystring
     )
  ++ Option.getWithDefault(url.hash, "");
};

let make =
    (
      ~protocol=?,
      ~slashes=false,
      ~host=?,
      ~hostname=?,
      ~port=?,
      ~auth=?,
      ~username=?,
      ~password=?,
      ~pathname=?,
      ~querystring=?,
      ~hash=?,
      (),
    ) => {
  let relative = Option.isNone(protocol) && slashes === false;

  /* TODO: parse query */
  /* TODO: resolve relative */

  let (host, port) =
    requiresPort(port, protocol) === false ?
      (hostname, None) : (host, port);

  let (username, password) =
    switch (auth) {
    | Some(auth) =>
      let result = Str.split(auth, ':');
      (result[0], result[1]);
    | None => (username, password)
    };

  let origin =
    switch (protocol, host) {
    | (Some(protocol), Some(host)) when protocol !== "file:" =>
      Some(protocol ++ "//" ++ host)
    | _ => None
    };

  let url = {
    /* Temporary placeholder of href so we can call toString on the url */
    href: "",
    protocol,
    slashes,
    origin,
    host,
    hostname,
    port,
    auth,
    username,
    password,
    pathname,
    querystring,
    hash,
  };

  {...url, href: toString(url)};
};

let fromString = address => {
  let (protocol, slashes, rest) = extractProtocol(address);

  let rest = Option.getWithDefault(rest, "");

  let [hostname, port, host, auth, pathname, querystring, hash] =
    Rules.exec(rest);

  /* TODO: handle int_of_string throw */
  let port = Option.map(port, port => int_of_string(port));

  make(
    ~protocol?,
    ~slashes,
    ~host?,
    ~hostname?,
    ~port?,
    ~auth?,
    ~pathname?,
    ~querystring?,
    ~hash?,
    (),
  );
};
