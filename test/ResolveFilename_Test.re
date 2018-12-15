/* Tests originally from https://github.com/mjackson/resolve-pathname/blob/20152400021377da9aa5e25fc0846ebe2d17fdcb/modules/__tests__/resolvePathname-test.js */

open Jest;
open Expect;

let resolvePathname = Url.resolvePathname;

describe("resolvePathname", () => {
  test("works when from is not given", () =>
    expect(resolvePathname(~to_="c", ())) |> toEqual("c")
  );

  test("works when from is relative", () =>
    expect(resolvePathname(~to_="c", ~from="a/b", ())) |> toEqual("a/c")
  );

  test("works when to is absolute", () =>
    expect(resolvePathname(~to_="/c", ~from="/a/b", ())) |> toEqual("/c")
  );

  /* TODO: handle empty string? */
  test("works when to is empty", () =>
    expect(resolvePathname(~from="/a/b", ())) |> toEqual("/a/b")
  );

  test("works when to is a sibling of the parent", () =>
    expect(resolvePathname(~to_="../c", ~from="/a/b", ())) |> toEqual("/c")
  );

  test("works when to is a sibling path", () =>
    expect(resolvePathname(~to_="c", ~from="/a/b", ())) |> toEqual("/a/c")
  );

  test("works when from is an index path", () =>
    expect(resolvePathname(~to_="c", ~from="/a/", ())) |> toEqual("/a/c")
  );

  test("works when to points to the parent directory", () =>
    expect(resolvePathname(~to_="..", ~from="/a/b", ())) |> toEqual("/")
  );

  /* Copied from node's test/parallel/test-url.js */
  let nodeURLResolveTestCases = [
    ("/foo/bar/baz", "quux", "/foo/bar/quux"),
    ("/foo/bar/baz", "quux/asdf", "/foo/bar/quux/asdf"),
    ("/foo/bar/baz", "quux/baz", "/foo/bar/quux/baz"),
    ("/foo/bar/baz", "../quux/baz", "/foo/quux/baz"),
    ("/foo/bar/baz", "/bar", "/bar"),
    ("/foo/bar/baz/", "quux", "/foo/bar/baz/quux"),
    ("/foo/bar/baz/", "quux/baz", "/foo/bar/baz/quux/baz"),
    ("/foo/bar/baz", "../../../../../../../../quux/baz", "/quux/baz"),
    ("/foo/bar/baz", "../../../../../../../quux/baz", "/quux/baz"),
    ("/foo", ".", "/"),
    ("/foo", "..", "/"),
    ("/foo/", ".", "/foo/"),
    ("/foo/", "..", "/"),
    ("/foo/bar", ".", "/foo/"),
    ("/foo/bar", "..", "/"),
    ("/foo/bar/", ".", "/foo/bar/"),
    ("/foo/bar/", "..", "/foo/"),
    ("foo/bar", "../../../baz", "../../baz"),
    ("foo/bar/", "../../../baz", "../baz"),
    /* ("http://example.com/b//c//d;p?q#blarg", "https:#hash2", "https:///#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "https:/p/a/t/h?s#hash2", */
    /* "https://p/a/t/h?s#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "https://u:p@h.com/p/a/t/h?s#hash2", */
    /* "https://u:p@h.com/p/a/t/h?s#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "https:/a/b/c/d", */
    /* "https://a/b/c/d"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "http:#hash2", */
    /* "http://example.com/b//c//d;p?q#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "http:/p/a/t/h?s#hash2", */
    /* "http://example.com/p/a/t/h?s#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "http://u:p@h.com/p/a/t/h?s#hash2", */
    /* "http://u:p@h.com/p/a/t/h?s#hash2"), */
    /* ("http://example.com/b//c//d;p?q#blarg", */
    /* "http:/a/b/c/d", */
    /* "http://example.com/a/b/c/d"), */
    ("/foo/bar/baz", "/../etc/passwd", "/etc/passwd"),
    /* ("http://localhost", "file:///Users/foo", "file:///Users/foo"), */
    /* ("http://localhost", "file://foo/Users", "file://foo/Users"), */
  ];

  testAll(
    "resolvePathname(~to_, ~from) === expected",
    nodeURLResolveTestCases,
    ((from, to_, expected)) =>
    expect(resolvePathname(~to_, ~from, ())) |> toEqual(expected)
  );
});
