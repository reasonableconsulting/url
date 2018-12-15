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
  hash: option(string),
};

let empty: t;

let resolvePathname: (~to_: string=?, ~from: string=?, unit) => string;

let toString: t => string;

let makeWithLocation:
  (
    ~protocol: string=?,
    ~slashes: bool=?,
    ~host: string=?,
    ~hostname: string=?,
    ~port: int=?,
    ~auth: string=?,
    ~username: string=?,
    ~password: string=?,
    ~pathname: string=?,
    ~querystring: string=?,
    ~hash: string=?,
    option(t)
  ) =>
  t;

let make:
  (
    ~protocol: string=?,
    ~slashes: bool=?,
    ~host: string=?,
    ~hostname: string=?,
    ~port: int=?,
    ~auth: string=?,
    ~username: string=?,
    ~password: string=?,
    ~pathname: string=?,
    ~querystring: string=?,
    ~hash: string=?,
    unit
  ) =>
  t;

let fromString: (~location: t=?, string) => t;
