<p align="left">
  <a href="https://reasonable.consulting/">
    <img height="270" width="270" src="https://reasonable.consulting/images/reasonable-logo.png">
  </a>
</p>

# @reasonableconsulting/url

Work with URLs in ReasonML.

[![Travis Build Status][travis-image]][travis-url] [![AppVeyor Build Status][appveyor-image]][appveyor-url]

## Usage

```reason
let url = Url.fromString("http://google.com/");
Js.log(url.pathname === Some("/"));

let str = Url.toString(url);
Js.log(str === "http://google.com/");

/* Or with base location */
let url = Url.fromString(~location=Url.fromString("http://google.com"), "/foo?foo=bar");

Js.log(url.href === "http://google.com/foo?foo=bar");
```

## API

### `type t`

The `Url` type. Has the shape:
```reason
{
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
}
```

### `empty`

A completely empty `Url.t`.

### `toString(url)`

Turns a `Url.t` into a string representation.

### `fromString(~location: t=?, string)`

Turns a string into a `Url.t`. Optionally, takes a "base" `Url.t` as `~location`.

### `make(~protocol: string=?, ~slashes: bool=?, ~host: string=?, ~hostname: string=?, ~port: int=?, ~auth: string=?, ~username: string=?, ~password: string=?, ~pathname: string=?, ~querystring: string=?, ~hash: string=?, unit)`

Makes a `Url.t` from all the optional arguments. Useful if you are building up a Url to later stringify.

### `makeWithLocation(~protocol: string=?, ~slashes: bool=?, ~host: string=?, ~hostname: string=?, ~port: int=?, ~auth: string=?, ~username: string=?, ~password: string=?, ~pathname: string=?, ~querystring: string=?, ~hash: string=?, option(t))`

Makes a `Url.t` from all the optional arguments and an `option(Url.t)` representing the "base" location. This method is mostly used internally but made available for other use cases.

## Inspiration

The `resolvePathname` implementation is a rewrite of [mjackson/resolve-pathname](https://github.com/mjackson/resolve-pathname) - [Originally licensed](https://github.com/mjackson/resolve-pathname/blob/42f1e54748e5d8d861deb0a1f05d5c188fa10b53/LICENSE), MIT License - Copyright (c) Michael Jackson 2016-2018.

The `fromString`, `toString`, and `make` implementations are a rewrite of [unshiftio/url-parse](https://github.com/unshiftio/url-parse) - [Originally licensed](https://github.com/unshiftio/url-parse/blob/6c5842fb07778f6d624f0f886548ce4b40d843d4/LICENSE), MIT License - Copyright (c) 2015 Unshift.io, Arnout Kazemier, the Contributors.

[travis-url]: https://travis-ci.org/reasonableconsulting/url
[travis-image]: http://img.shields.io/travis/reasonableconsulting/url.svg?label=travis-ci

[appveyor-url]: https://ci.appveyor.com/project/reasonableconsulting/url
[appveyor-image]: https://img.shields.io/appveyor/ci/reasonableconsulting/url.svg?label=appveyor
