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
  let isSome = opt =>
    switch (opt) {
    | Some(_) => true
    | None => false
    };

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

  let flatMap = (opt, fn) =>
    switch (opt) {
    | Some(value) => fn(value)
    | None => None
    };
};

module Arr = {
  let pop = arr =>
    switch (Array.sub(arr, 0, Array.length(arr) - 1)) {
    | exception (Invalid_argument(_)) => arr
    | arr => arr
    };

  let tail = arr =>
    switch (Array.sub(arr, 1, Array.length(arr) - 1)) {
    | exception (Invalid_argument(_)) => [||]
    | arr => arr
    };

  let first = arr =>
    switch (arr[0]) {
    | exception (Invalid_argument(_)) => None
    | el => Some(el)
    };

  let last = arr =>
    switch (arr[Array.length(arr) - 1]) {
    | exception (Invalid_argument(_)) => None
    | el => Some(el)
    };

  let isEmpty = arr => Array.length(arr) === 0;

  let reduce = (arr, start, fn) => Array.fold_left(fn, start, arr);

  let reduceRight = (arr, start, fn) => {
    Array.fold_right((part, result) => fn(result, part), arr, start);
  };

  let join = (arr, sep) =>
    switch (first(arr)) {
    | None => ""
    | Some(first) =>
      reduce(tail(arr), first, (result, part) => result ++ sep ++ part)
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

  let rec split = (str, chr) => {
    switch (index(str, chr)) {
    | Some(index) =>
      let beginChunk = Option.getWithDefault(slice(str, 0, index), "");
      let endChunk = Option.getWithDefault(sliceToEnd(str, index + 1), "");
      Array.append([|beginChunk|], split(endChunk, chr));
    | None => [|str|]
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

  let toInt = str =>
    switch (int_of_string(str)) {
    | exception (Failure(_)) => None
    | value => Some(value)
    };
};

/* TODO: How does these behave with a native compilation target? */
let protocolRegExp = [%bs.re {|/^([a-z][a-z0-9.+-]*:)?(\/\/)?([\S\s]*)/i|}];
let slashesRegExp = [%bs.re {|/^[A-Za-z][A-Za-z0-9+-.]*:\/\//|}];
let portRegExp = [%bs.re {|/:(\d+)$/|}];
let pathnameRegExp = [%bs.re {|/(.*)/|}];

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

  let pathnameWhenSlashes = address => {
    let address = sanitize(address);

    switch (Str.index(address, '/')) {
    | Some(index) => Str.chunkEnd(address, index)
    | None => (None, address)
    };
  };

  let pathnameWhenNoSlashes = address => {
    let match = Js.Re.exec(address, pathnameRegExp);

    switch (match) {
    | Some(result) =>
      let captured = Js.Re.captures(result);

      let pathname =
        switch (Js.Nullable.toOption(captured[1])) {
        | Some(pathname) => Some(pathname)
        | None => None
        };

      switch (Str.slice(address, 0, Js.Re.index(result))) {
      | Some(address) => (pathname, address)
      | None => (pathname, "")
      };
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

  let rec exec = (~rules, ~results, address) =>
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
    let protocol = Arr.first(Str.split(protocol, ':'));

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

let isAbsolute = str => Str.startsWith(str, '/');

/* TODO: This needs massive cleanup */
/* Logic from https://github.com/mjackson/resolve-pathname/blob/master/modules/index.js */
let resolvePathname = (~to_=?, ~from=?, ()) => {
  let toParts = Option.mapWithDefault(to_, [||], to_ => Str.split(to_, '/'));
  let fromParts =
    Option.mapWithDefault(from, [||], from => Str.split(from, '/'));

  let isToAbsolute = Option.mapWithDefault(to_, false, isAbsolute);
  let isFromAbsolute = Option.mapWithDefault(from, false, isAbsolute);
  let mustEndAbsolute = isToAbsolute || isFromAbsolute;

  let fromParts =
    if (isToAbsolute) {
      toParts;
    } else if (Option.isSome(to_)) {
      Arr.pop(fromParts)->Array.append(toParts);
    } else {
      fromParts;
    };

  if (Arr.isEmpty(fromParts)) {
    "/";
  } else {
    let lastSegment = Arr.last(fromParts);
    let needsTrailingSlash =
      switch (lastSegment) {
      | Some(".")
      | Some("..")
      | Some("") => true
      | _ => false
      };

    let up = ref(0);
    let fromParts =
      Arr.reduceRight(fromParts, [||], (result, part) =>
        switch (part) {
        | "." => result
        | ".." =>
          up := up^ + 1;
          result;
        | part =>
          if (up^ > 0) {
            up := up^ - 1;
            result;
          } else {
            Array.append([|part|], result);
          }
        }
      );

    let fromParts =
      if (!mustEndAbsolute) {
        Array.append(Array.make(up^, ".."), fromParts);
      } else {
        fromParts;
      };

    let fromParts =
      if (mustEndAbsolute) {
        switch (Arr.first(fromParts)) {
        | Some(part) =>
          part !== "" && !isAbsolute(part) ?
            Array.append([|""|], fromParts) : fromParts
        | None => Array.append([|""|], fromParts)
        };
      } else {
        fromParts;
      };

    let result = Arr.join(fromParts, "/");

    if (needsTrailingSlash && !Str.endsWith(result, '/')) {
      result ++ "/";
    } else {
      result;
    };
  };
};

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
  /* let pathname =
     if (relative /* && location.slashes
            && !Str.startsWith(url.pathname, '/')
            && (pathname !== '' || location.pathname !== '') */) {
       resolvePathname(pathname, location.pathname);
     }; */

  let (host, port) =
    requiresPort(port, protocol) === false ?
      (hostname, None) : (host, port);

  let (username, password) =
    switch (auth) {
    | Some(auth) =>
      /* TODO: this probably can be cleaned up */
      switch (Str.split(auth, ':')) {
      | [|username, password|] => (Some(username), Some(password))
      | [|username|] => (Some(username), None)
      | _ => (username, password)
      }
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
    Rules.exec(
      ~rules=[
        Rules.hash,
        Rules.querystring,
        slashes ? Rules.pathnameWhenSlashes : Rules.pathnameWhenNoSlashes,
        Rules.auth,
        Rules.host,
        Rules.port,
        Rules.hostname,
      ],
      ~results=[],
      rest,
    );

  let port = Option.flatMap(port, Str.toInt);

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
