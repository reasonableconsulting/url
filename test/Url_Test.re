open Jest;
open Expect;

describe("url-parse", () => {
  /* test("defaults to empty address to return valid URL instance", () => {
       let url = Url.fromString();

       expect(url) |> toBe("object");
       expect(url.pathname) |> toBe("string");
       expect(url.host) |> toBe("string");
       expect(url.hostname) |> toBe("string");
     }); */

  /* test("works when the global variable is not defined", () => {
       let globallet = global;
       global = undefined;
       let url = Url.fromString("http://google.com/?foo=bar", true);

       expect(url) |> toBe("object");
       expect(url.pathname) |> toBe("string");
       expect(url.host) |> toBe("string");
       expect(url.hostname) |> toBe("string");

       global = globalVar;
     }); */

  /* describe("extractProtocol", () => {
       test("extracts the protocol data", () =>
         expect(parse.extractProtocol("")).eql({
           slashes: false,
           protocol: "",
           rest: "",
         })
       );

       test("does not truncate the input string", () => {
         let input = "foo\nbar\rbaz\u2028qux\u2029";

         expect(parse.extractProtocol(input)).eql({
           slashes: false,
           protocol: "",
           rest: input,
         });
       });
     }); */

  /* test("parses the query string into an object", () => {
       let url = "http://google.com/?foo=bar";
       let data = Url.fromString(url, true);

       expect(data.query) |> toBe("object");
       expect(data.query.foo) |> toEqual("bar");

       url = "http://google.com/";
       data = Url.fromString(url, true);

       expect(data.query) |> toBe("object");
       expect(data.query).is.empty();
     }); */

  test("does not add question mark to href if query string is empty", () => {
    let url = "http://google.com/";
    let data = Url.fromString(url);

    expect(data.href) |> toEqual(url);
  });

  /* test("allows a custom function as parser", () => {
       let url = "http://google.com/?foo=bar";
       let data = Url.fromString(url, () => "1337");

       expect(data.query) |> toEqual("1337");
     }); */

  /* test("allows a custom stringify function", () => {
       let url = "http://google.com/?foo=bar";
       let data = Url.fromString(url, true);

       let str = data.toString(() => "lolcakes");
       expect(str) |> toEqual("http://google.com/?lolcakes");
     }); */

  /* test("allows a custom location object", () => {
       let url = "/foo?foo=bar";
       let data = Url.fromString(url, Url.fromString("http://google.com"));

       expect(data.href) |> toEqual("http://google.com/foo?foo=bar");
     }); */

  /* test("is blob: location aware", () => {
       let blob = {
         "href": "blob:https%3A//gist.github.com/3f272586-6dac-4e29-92d0-f674f2dde618",
         "pathname": "https%3A//gist.github.com/3f272586-6dac-4e29-92d0-f674f2dde618",
         "origin": "https://gist.github.com",
         "protocol": "blob:",
         "hostname": "",
         "search": "",
         "hash": "",
         "host": "",
         "port": "",
       };

       let url = "/unshiftio/url-parse";
       let data = Url.fromString(url, blob);

       expect(data.href)
       |> toEqual("https://gist.github.com/unshiftio/url-parse");
     }); */

  /* test("can parse complex urls multiple times without errors", () => {
       let url = "https://www.mozilla.org/en-US/firefox/34.0/whatsnew/?oldversion=33.1";

       for (let i = 0; i < 100; i++) {
         Url.fromString(url);
       }
     }); */

  test("converts hostname to lowercase", () => {
    let url = "HTTP://fOo.eXaMPle.com";

    expect(Url.fromString(url).hostname)
    |> toEqual(Some("foo.example.com"));
  });

  test("does not lowercase the path", () => {
    let url = "HTTP://X.COM/Y/Z";

    expect(Url.fromString(url).pathname) |> toEqual(Some("/Y/Z"));
  });

  describe("removes default port numbers", () => {
    let url = "http://example.com:80";
    let parsed = Url.fromString(url);

    test("port", () =>
      expect(parsed.port) |> toEqual(None)
    );
    test("host", () =>
      expect(parsed.host) |> toEqual(Some("example.com"))
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(Some("example.com"))
    );
    test("href", () =>
      expect(parsed.href) |> toEqual("http://example.com")
    );
  });

  describe("understands an / as pathname", () => {
    let url = "http://example.com:80/";
    let parsed = Url.fromString(url);

    test("port", () =>
      expect(parsed.port) |> toEqual(None)
    );
    test("username", () =>
      expect(parsed.username) |> toEqual(None)
    );
    test("password", () =>
      expect(parsed.password) |> toEqual(None)
    );
    test("pathname", () =>
      expect(parsed.pathname) |> toEqual(Some("/"))
    );
    test("host", () =>
      expect(parsed.host) |> toEqual(Some("example.com"))
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(Some("example.com"))
    );
    test("href", () =>
      expect(parsed.href) |> toEqual("http://example.com/")
    );
  });

  describe("does not care about spaces", () => {
    let url = "http://x.com/path?that\"s#all, folks";
    let parsed = Url.fromString(url);

    test("port", () =>
      expect(parsed.port) |> toEqual(None)
    );
    test("username", () =>
      expect(parsed.username) |> toEqual(None)
    );
    test("password", () =>
      expect(parsed.password) |> toEqual(None)
    );
    test("pathname", () =>
      expect(parsed.pathname) |> toEqual(Some("/path"))
    );
    test("hash", () =>
      expect(parsed.hash) |> toEqual(Some("#all, folks"))
    );
    test("querystring", () =>
      expect(parsed.querystring) |> toEqual(Some("?that\"s"))
    );
    test("host", () =>
      expect(parsed.host) |> toEqual(Some("x.com"))
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(Some("x.com"))
    );
  });

  describe("accepts + in the url", () => {
    let url = "http://x.y.com+a/b/c";
    let parsed = Url.fromString(url);

    test("protocol", () =>
      expect(parsed.protocol) |> toEqual(Some("http:"))
    );
    test("host", () =>
      expect(parsed.host) |> toEqual(Some("x.y.com+a"))
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(Some("x.y.com+a"))
    );
    test("pathname", () =>
      expect(parsed.pathname) |> toEqual(Some("/b/c"))
    );
  });

  describe("ignores \\ in pathnames", () => {
    let url = "http://google.com:80\\@yahoo.com/#what\\is going on";
    let parsed = Url.fromString(url);

    test("port", () =>
      expect(parsed.port) |> toEqual(None)
    );
    test("username", () =>
      expect(parsed.username) |> toEqual(None)
    );
    test("password", () =>
      expect(parsed.password) |> toEqual(None)
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(Some("google.com"))
    );
    test("hash", () =>
      expect(parsed.hash) |> toEqual(Some("#what\\is going on"))
    );

    let parsed = Url.fromString("//\\what-is-up.com");
    test("pathname", () =>
      expect(parsed.pathname) |> toEqual(Some("/what-is-up.com"))
    );
  });

  describe("correctly ignores multiple slashes //", () => {
    let url = "////what-is-up.com";
    let parsed = Url.fromString(url);

    test("host", () =>
      expect(parsed.host) |> toEqual(None)
    );
    test("hostname", () =>
      expect(parsed.hostname) |> toEqual(None)
    );
  });

  describe("origin", () => {
    test("generates an origin property", () => {
      let url = "http://google.com:80/pathname";
      let parsed = Url.fromString(url);

      expect(parsed.origin) |> toEqual(Some("http://google.com"));
    });

    test("is lowercased", () => {
      let url = "HTTP://gOogle.cOm:80/pathname";
      let parsed = Url.fromString(url);

      expect(parsed.origin) |> toEqual(Some("http://google.com"));
    });

    /* test("sets null if no hostname is specified", () => {
         let url = "http://";
         let parsed = Url.fromString(url, {});

         expect(parsed.origin) |> toEqual("null");
       }); */

    testAll(
      "removes default ports for http",
      [
        ("http://google.com:80/pathname", "http://google.com"),
        ("http://google.com:80", "http://google.com"),
        ("http://google.com", "http://google.com"),
        ("https://google.com:443/pathname", "https://google.com"),
        ("http://google.com:443/pathname", "http://google.com:443"),
        ("https://google.com:80/pathname", "https://google.com:80"),
      ],
      ((url, origin)) => {
        let o = Url.fromString(url);
        expect(o.origin) |> toEqual(Some(origin));
      },
    );

    /* test("handles file:// based urls as null", () => {
         let o = Url.fromString("file://google.com/pathname");
         expect(o.origin) |> toEqual("null");
       }); */

    testAll(
      "removes default ports for ws",
      [
        ("ws://google.com:80/pathname", "ws://google.com"),
        ("wss://google.com:443/pathname", "wss://google.com"),
        ("ws://google.com:443/pathname", "ws://google.com:443"),
        ("wss://google.com:80/pathname", "wss://google.com:80"),
      ],
      ((url, origin)) => {
        let o = Url.fromString(url);
        expect(o.origin) |> toEqual(Some(origin));
      },
    );

    describe("maintains the port number for non-default port numbers", () => {
      let parsed = Url.fromString("http://google.com:8080/pathname");

      test("host", () =>
        expect(parsed.host) |> toEqual(Some("google.com:8080"))
      );
      test("href", () =>
        expect(parsed.href) |> toEqual("http://google.com:8080/pathname")
      );
    });
  });

  describe("protocol", () => {
    testAll(
      "extracts the right protocol from a url",
      [
        ("http://example.com", Some("http:")),
        ("mailto:test@example.com", Some("mailto:")),
        (
          "data:text/html,%3Ch1%3EHello%2C%20World!%3C%2Fh1%3E",
          Some("data:"),
        ),
        ("sip:alice@atlanta.com", Some("sip:")),
      ],
      ((href, protocol)) => {
        let data = Url.fromString(href);
        expect(data.protocol) |> toEqual(protocol);
      },
    );

    testAll(
      "extracts the right path from protocol from a url",
      [
        ("http://example.com", None),
        ("mailto:test@example.com", Some("test@example.com")),
        (
          "data:text/html,%3Ch1%3EHello%2C%20World!%3C%2Fh1%3E",
          Some("text/html,%3Ch1%3EHello%2C%20World!%3C%2Fh1%3E"),
        ),
        ("sip:alice@atlanta.com", Some("alice@atlanta.com")),
      ],
      ((href, pathname)) => {
        let data = Url.fromString(href);
        expect(data.pathname) |> toEqual(pathname);
      },
    );

    test("converts protocol to lowercase", () => {
      let url = "HTTP://example.com";

      expect(Url.fromString(url).protocol) |> toEqual(Some("http:"));
    });
    /* test("correctly adds ':' to protocol in final url string", () => {
         let data = Url.fromString("google.com/foo", {});
         data.set("protocol", "https");
         expect(data.href) |> toEqual("https://google.com/foo");

         data = Url.fromString("https://google.com/foo");
         data.protocol = "http";
         expect(data.toString()) |> toEqual("http://google.com/foo");

         data = Url.fromString("http://google.com/foo");
         data.set("protocol", "https:");
         expect(data.href) |> toEqual("https://google.com/foo");
       }); */
  });

  describe("ip", () => {
    describe("parses ipv6", () => {
      let url = "http://[1080:0:0:0:8:800:200C:417A]:61616/foo/bar?q=z";
      let parsed = Url.fromString(url);

      test("port", () =>
        expect(parsed.port) |> toEqual(Some(61616))
      );
      test("querystring", () =>
        expect(parsed.querystring) |> toEqual(Some("?q=z"))
      );
      test("protocol", () =>
        expect(parsed.protocol) |> toEqual(Some("http:"))
      );
      test("hostname", () =>
        expect(parsed.hostname)
        |> toEqual(Some("[1080:0:0:0:8:800:200c:417a]"))
      );
      test("pathname", () =>
        expect(parsed.pathname) |> toEqual(Some("/foo/bar"))
      );
      test("href", () =>
        expect(parsed.href)
        |> toEqual("http://[1080:0:0:0:8:800:200c:417a]:61616/foo/bar?q=z")
      );
    });

    describe("parses ipv6 with auth", () => {
      let url = "http://user:password@[3ffe:2a00:100:7031::1]:8080";
      let parsed = Url.fromString(url);

      test("username", () =>
        expect(parsed.username) |> toEqual(Some("user"))
      );
      test("password", () =>
        expect(parsed.password) |> toEqual(Some("password"))
      );
      test("host", () =>
        expect(parsed.host) |> toEqual(Some("[3ffe:2a00:100:7031::1]:8080"))
      );
      test("hostname", () =>
        expect(parsed.hostname) |> toEqual(Some("[3ffe:2a00:100:7031::1]"))
      );
      test("href", () =>
        expect(parsed.href) |> toEqual(url)
      );
    });

    describe("parses ipv4", () => {
      let url = "http://222.148.142.13:61616/foo/bar?q=z";
      let parsed = Url.fromString(url);

      test("port", () =>
        expect(parsed.port) |> toEqual(Some(61616))
      );
      test("querystring", () =>
        expect(parsed.querystring) |> toEqual(Some("?q=z"))
      );
      test("protocol", () =>
        expect(parsed.protocol) |> toEqual(Some("http:"))
      );
      test("hostname", () =>
        expect(parsed.hostname) |> toEqual(Some("222.148.142.13"))
      );
      test("pathname", () =>
        expect(parsed.pathname) |> toEqual(Some("/foo/bar"))
      );
      test("href", () =>
        expect(parsed.href) |> toEqual(url)
      );
    });
  });

  describe("auth", () => {
    describe("does not lowercase the USER:PASS", () => {
      let url = "HTTP://USER:PASS@EXAMPLE.COM";
      let parsed = Url.fromString(url);

      test("username", () =>
        expect(parsed.username) |> toEqual(Some("USER"))
      );
      test("password", () =>
        expect(parsed.password) |> toEqual(Some("PASS"))
      );
      test("protocol", () =>
        expect(parsed.protocol) |> toEqual(Some("http:"))
      );
      test("host", () =>
        expect(parsed.host) |> toEqual(Some("example.com"))
      );
      test("hostname", () =>
        expect(parsed.hostname) |> toEqual(Some("example.com"))
      );
    });

    describe("accepts @ in pathnames", () => {
      let url = "http://mt0.google.com/vt/lyrs=m@114&hl=en&src=api&x=2&y=2&z=3&s=";
      let parsed = Url.fromString(url);

      test("pathname", () =>
        expect(parsed.pathname)
        |> toEqual(Some("/vt/lyrs=m@114&hl=en&src=api&x=2&y=2&z=3&s="))
      );
      test("username", () =>
        expect(parsed.username) |> toEqual(None)
      );
      test("password", () =>
        expect(parsed.password) |> toEqual(None)
      );
    });

    describe("does not require passwords for auth", () => {
      let url = "http://user@www.example.com/";
      let parsed = Url.fromString(url);

      test("password", () =>
        expect(parsed.password) |> toEqual(None)
      );
      test("pathname", () =>
        expect(parsed.pathname) |> toEqual(Some("/"))
      );
      test("username", () =>
        expect(parsed.username) |> toEqual(Some("user"))
      );
      test("protocol", () =>
        expect(parsed.protocol) |> toEqual(Some("http:"))
      );
      test("hostname", () =>
        expect(parsed.hostname) |> toEqual(Some("www.example.com"))
      );
      test("href", () =>
        expect(parsed.href) |> toEqual(url)
      );
    });
  });

  test("accepts multiple ???", () => {
    let url = "http://mt0.google.com/vt/lyrs=m@114???&hl=en&src=api&x=2&y=2&z=3&s=";
    expect(Url.fromString(url).querystring)
    |> toEqual(Some("???&hl=en&src=api&x=2&y=2&z=3&s="));
  });
  /* test("accepts a string as source argument", () => {
       let data =
         Url.fromString("/foo", "http://sub.example.com/bar?foo=bar#hash");

       expect(data.port) |> toEqual("");
       expect(data.host) |> toEqual("sub.example.com");
       expect(data.href) |> toEqual("http://sub.example.com/foo");
     }); */
  /* describe("inheritance", () => {
       test("does not inherit port numbers for non relative urls", () => {
         let data =
           Url.fromString(
             "http://localhost",
             Url.fromString("http://sub.example.com:808/"),
           );

         expect(data.port) |> toEqual("");
         expect(data.host) |> toEqual("localhost");
         expect(data.href) |> toEqual("http://localhost");
       });

       test("inherits port numbers for relative urls", () => {
         let data =
           Url.fromString(
             "/foo",
             Url.fromString("http://sub.example.com:808/"),
           );

         expect(data.port) |> toEqual("808");
         expect(data.hostname) |> toEqual("sub.example.com");
         expect(data.host) |> toEqual("sub.example.com:808");
         expect(data.href) |> toEqual("http://sub.example.com:808/foo");
       });

       test("inherits slashes for relative urls", () => {
         let data =
           Url.fromString(
             "/foo",
             {
               hash: "",
               host: "example.com",
               hostname: "example.com",
               href: "http://example.com/",
               origin: "http://example.com",
               password: "",
               pathname: "/",
               port: "",
               protocol: "http:",
               search: "",
             },
           );

         expect(data.slashes) |> toEqual(true);
         expect(data.href) |> toEqual("http://example.com/foo");

         data =
           Url.fromString(
             "/foo",
             {
               auth: null,
               hash: null,
               host: "example.com",
               hostname: "example.com",
               href: "http://example.com/",
               path: "/",
               pathname: "/",
               port: null,
               protocol: "http:",
               query: null,
               search: null,
               slashes: true,
             },
           );

         expect(data.slashes) |> toEqual(true);
         expect(data.href) |> toEqual("http://example.com/foo");
       });

       test("inherits protocol for relative protocols", () => {
         let data =
           Url.fromString(
             "//foo.com/foo",
             Url.fromString("http://sub.example.com:808/"),
           );

         expect(data.port) |> toEqual("");
         expect(data.host) |> toEqual("foo.com");
         expect(data.protocol) |> toEqual("http:");
         expect(data.href) |> toEqual("http://foo.com/foo");
       });

       test("does not inherit pathname for non relative urls", () => {
         let data =
           Url.fromString(
             "http://localhost",
             Url.fromString("http://foo:bar@sub.example.com/bar?foo=bar#hash"),
           );

         expect(data.port) |> toEqual("");
         expect(data.host) |> toEqual("localhost");
         expect(data.href) |> toEqual("http://localhost");
       });

       /* test("resolves pathname for relative urls", () => {
            let data, i = 0;
            let tests = [
              [", "http://foo.com", ""],
              [", "http://foo.com/", "/"],
              ["a", "http://foo.com", "/a"],
              ["a/", "http://foo.com", "/a/"],
              ["b/c", "http://foo.com/a", "/b/c"],
              ["b/c", "http://foo.com/a/", "/a/b/c"],
              [".", "http://foo.com", "/"],
              ["./", "http://foo.com", "/"],
              ["./.", "http://foo.com", "/"],
              [".", "http://foo.com/a", "/"],
              [".", "http://foo.com/a/", "/a/"],
              ["./", "http://foo.com/a/", "/a/"],
              ["./.", "http://foo.com/a/", "/a/"],
              ["./b", "http://foo.com/a/", "/a/b"],
              ["..", "http://foo.com", "/"],
              ["../", "http://foo.com", "/"],
              ["../..", "http://foo.com", "/"],
              ["..", "http://foo.com/a/b", "/"],
              ["..", "http://foo.com/a/b/", "/a/"],
              ["../..", "http://foo.com/a/b", "/"],
              ["../..", "http://foo.com/a/b/", "/"],
              ["../../../../c", "http://foo.com/a/b/", "/c"],
              ["./../d", "http://foo.com/a/b/c", "/a/d"],
              ["d/e/f/./../../g", "http://foo.com/a/b/c", "/a/b/d/g"]
            ];

            for (; i < tests.length; i++) {
              data = Url.fromString(tests[i][0], tests[i][1]);
              expect(data.pathname) |> toEqual(tests[i][2]);
            }
          }); */

       test("does not inherit hashes and query strings from source object", () => {
         let data =
           Url.fromString(
             "/foo",
             Url.fromString("http://sub.example.com/bar?foo=bar#hash"),
           );

         expect(data.port) |> toEqual("");
         expect(data.host) |> toEqual("sub.example.com");
         expect(data.href) |> toEqual("http://sub.example.com/foo");
       });

       test("does not inherit auth from source object", () => {
         let base = Url.fromString("http://foo:bar@sub.example.com");
         let data = Url.fromString("/foo", base);

         expect(data.port) |> toEqual("");
         expect(data.username) |> toEqual("");
         expect(data.password) |> toEqual("");
         expect(data.host) |> toEqual("sub.example.com");
         expect(data.href) |> toEqual("http://sub.example.com/foo");
       });
     }); */
  /* describe("#set", () => {
       test("correctly updates the host when setting port", () => {
         let data = Url.fromString("http://google.com/foo");

         expect(data.set("port", 8080)) |> toEqual(data);

         expect(data.host) |> toEqual("google.com:8080");
         expect(data.href) |> toEqual("http://google.com:8080/foo");
       });

       test("correctly updates the host when setting port (IPv6)", () => {
         let data = Url.fromString("http://[7886:3423::1233]/foo");

         expect(data.set("port", 8080)) |> toEqual(data);

         expect(data.host) |> toEqual("[7886:3423::1233]:8080");
         expect(data.href) |> toEqual("http://[7886:3423::1233]:8080/foo");
       });

       /* test("removes querystring and hash", () => {
            let data = Url.fromString("https://thisanurl.com/?swag=yolo#representing");

            data.set("query", ");
            data.set("hash", ");

            expect(data.href) |> toEqual("https://thisanurl.com/");
          }); */

       test("only sets port when its not default", () => {
         let data = Url.fromString("http://google.com/foo");

         expect(data.set("port", 80)) |> toEqual(data);

         expect(data.host) |> toEqual("google.com");
         expect(data.href) |> toEqual("http://google.com/foo");

         expect(data.set("port", 443)) |> toEqual(data);
         expect(data.host) |> toEqual("google.com:443");
         expect(data.href) |> toEqual("http://google.com:443/foo");
       });

       test("only sets port when its not default (IPv6)", () => {
         let data = Url.fromString("http://[7886:3423::1233]/foo");

         expect(data.set("port", 80)) |> toEqual(data);

         expect(data.host) |> toEqual("[7886:3423::1233]");
         expect(data.href) |> toEqual("http://[7886:3423::1233]/foo");

         expect(data.set("port", 443)) |> toEqual(data);
         expect(data.host) |> toEqual("[7886:3423::1233]:443");
         expect(data.href) |> toEqual("http://[7886:3423::1233]:443/foo");
       });

       /* test("prepends / to pathname", () => {
            let url = Url.fromString();

            url
              .set("protocol", "http")
              .set("host", "example.com:80")
              .set("pathname", "will/get/slash/prepended");

            expect(url.pathname) |> toEqual("/will/get/slash/prepended");
            expect(url.href) |> toEqual("http://example.com:80/will/get/slash/prepended");

            url.set("pathname", "");

            expect(url.pathname) |> toEqual("");
            expect(url.href) |> toEqual("http://example.com:80");

            url.set("pathname", "/has/slash");

            expect(url.pathname) |> toEqual("/has/slash");
            expect(url.href) |> toEqual("http://example.com:80/has/slash");
          }); */

       test("updates query with object", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("query", {bar: "foo"})) |> toEqual(data);

         expect(data.query.foo) |> toEqual(undefined);
         expect(data.query.bar) |> toEqual("foo");

         expect(data.href) |> toEqual("http://google.com/?bar=foo");
       });

       test("updates query with a string", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("query", "bar=foo")) |> toEqual(data);

         expect(data.query.foo) |> toEqual(undefined);
         expect(data.query.bar) |> toEqual("foo");

         expect(data.href) |> toEqual("http://google.com/?bar=foo");

         expect(data.set("query", "?baz=foo")) |> toEqual(data);

         expect(data.query.bar) |> toEqual(undefined);
         expect(data.query.baz) |> toEqual("foo");

         expect(data.href) |> toEqual("http://google.com/?baz=foo");
       });

       test("allows custom parser when updating query", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("query", "bar=foo", () => "1337")) |> toEqual(data);

         expect(data.query) |> toEqual("1337");

         expect(data.href) |> toEqual("http://google.com/?1337");
       });

       test(
         "throws error when updating query, if custom parser is not a function",
         () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(() =>
           data.set("query", "bar=foo", "1337")
         ).throws(Error);

         /* `data` is unchanged. */
         expect(data.href) |> toEqual("http://google.com/?foo=bar");
       });

       test("prepends # to hash", () => {
         let data = Url.fromString("http://example.com");

         data.set("hash", "usage");

         expect(data.hash) |> toEqual("#usage");
         expect(data.href) |> toEqual("http://example.com#usage");

         data.set("hash", "#license");

         expect(data.hash) |> toEqual("#license");
         expect(data.href) |> toEqual("http://example.com#license");
       });

       test("updates the port when updating host", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("host", "yahoo.com:808")) |> toEqual(data);

         expect(data.hostname) |> toEqual("yahoo.com");
         expect(data.host) |> toEqual("yahoo.com:808");
         expect(data.port) |> toEqual("808");

         expect(data.href) |> toEqual("http://yahoo.com:808/?foo=bar");
       });

       test("updates the port when updating host (IPv6)", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("host", "[56h7::1]:808")) |> toEqual(data);

         expect(data.hostname) |> toEqual("[56h7::1]");
         expect(data.host) |> toEqual("[56h7::1]:808");
         expect(data.port) |> toEqual("808");

         expect(data.href) |> toEqual("http://[56h7::1]:808/?foo=bar");
       });

       test("unsets the port when port is missing (IPv6)", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("host", "[56h7::1]")) |> toEqual(data);

         expect(data.hostname) |> toEqual("[56h7::1]");
         expect(data.host) |> toEqual("[56h7::1]");
         expect(data.port) |> toEqual("");

         expect(data.href) |> toEqual("http://[56h7::1]/?foo=bar");
       });

       test("unsets the port when the port is missing from host", () => {
         let data = Url.fromString("http://google.com:8000/?foo=bar");

         expect(data.set("host", "yahoo.com")) |> toEqual(data);

         expect(data.hostname) |> toEqual("yahoo.com");
         expect(data.host) |> toEqual("yahoo.com");
         expect(data.port) |> toEqual("");

         expect(data.href) |> toEqual("http://yahoo.com/?foo=bar");
       });

       test("updates the host when updating hostname", () => {
         let data = Url.fromString("http://google.com:808/?foo=bar");

         expect(data.set("hostname", "yahoo.com")) |> toEqual(data);

         expect(data.hostname) |> toEqual("yahoo.com");
         expect(data.host) |> toEqual("yahoo.com:808");
         expect(data.port) |> toEqual("808");

         expect(data.href) |> toEqual("http://yahoo.com:808/?foo=bar");
       });

       test("updates slashes when updating protocol", () => {
         let data = Url.fromString("sip:alice@atlanta.com");

         expect(data.set("protocol", "https")) |> toEqual(data);

         expect(data.href) |> toEqual("https://alice@atlanta.com");

         expect(data.set("protocol", "mailto", true)) |> toEqual(data);

         expect(data.href) |> toEqual("mailto:alice@atlanta.com");
       });

       test("updates other values", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         expect(data.set("protocol", "https:")) |> toEqual(data);
         expect(data.protocol) |> toEqual("https:");
         expect(data.href) |> toEqual("https://google.com/?foo=bar");

         data.set("username", "foo");

         expect(data.username) |> toEqual("foo");
         expect(data.href) |> toEqual("https://foo@google.com/?foo=bar");
       });

       test("lowercases the required values", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         data.set("protocol", "HTTPS:");
         expect(data.protocol) |> toEqual("https:");
         expect(data.href) |> toEqual("https://google.com/?foo=bar");

         data.set("host", "GOOGLE.LOL");
         expect(data.host) |> toEqual("google.lol");
         expect(data.href) |> toEqual("https://google.lol/?foo=bar");

         data.set("hostname", "YAhOo.COm");
         expect(data.hostname) |> toEqual("yahoo.com");
         expect(data.href) |> toEqual("https://yahoo.com/?foo=bar");
       });

       test("correctly updates the origin when host/protocol/port changes", () => {
         let data = Url.fromString("http://google.com/?foo=bar");

         data.set("protocol", "HTTPS:");
         expect(data.protocol) |> toEqual("https:");
         expect(data.origin) |> toEqual("https://google.com");

         data.set("port", "1337");
         expect(data.port) |> toEqual("1337");
         expect(data.origin) |> toEqual("https://google.com:1337");

         data.set("protocol", "file:");
         expect(data.protocol) |> toEqual("file:");
         expect(data.origin) |> toEqual("null");
       });
     }); */
  /* describe("fuzzy", () => {
       let fuzz = require("./fuzzy")
       let times = 10;

       for (let i = 0; i < times; i++) {
         (function (spec) {
           test("parses: "+ spec.href, () => {
             let url = Url.fromString(spec.href)
             let prop;

             for (prop in spec) {
               expect(url[prop]) |> toEqual(spec[prop]);
             }
           });
         })(fuzz());
       }
     }); */
});
