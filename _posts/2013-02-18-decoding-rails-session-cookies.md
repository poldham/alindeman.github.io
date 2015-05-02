---
layout: post
title: "Decoding Rails Session Cookies"
comments: true
redirect_from: "/2013/02/18/decoding-rails-session-cookies.html"
---

An [Upgrading to Rails 4](http://www.upgradingtorails4.com/) reader emailed me
about this quote from the book:

> Rails 3 uses digitally signed cookies as the default store for sessions.
> Digitally signed cookies cannot be easily tampered with, but users can read
> the data that is being saved.

He asked:

> Would you say that's something trivial to do, or would it involve some decent
> amount of work? Just by accessing the Rails 3 cookies from my browser's
> inspector, I can't seem to find a way to read the content from them.

Great question! Let's try.

For this example, I setup a Rails application that sets a session key to a
constant value:

{% highlight ruby %}
class ApplicationController < ActionController::Base
  before_filter :add_value_to_session

  private

  def add_value_to_session
    session[:message] = "Hello World!"
  end
end
{% endhighlight %}

If I hit the Rails application with `curl -i` (include headers), I see:

{% highlight bash %}
$ curl -i http://localhost:3000/
HTTP/1.1 200 OK 
Set-Cookie: _railsapp_session=BAh7B0kiD3Nlc3Npb25faWQGOgZFRkkiJWYxZGNlMmNmYjFmYjBkODQ0NTc1ZWE3OTBjZmJmNTZkBjsAVEkiDG1lc3NhZ2UGOwBGSSIRSGVsbG8gV29ybGQhBjsARg%3D%3D--decd8e233744e2ba5a80481426e41d72a6685986; path=/; HttpOnly
{% endhighlight %}

The cookie data looks obfuscated, but in reality it's shrouded only by Base64
encoding.

Rack's session cookie methods can easily decode it:

{% highlight ruby %}
# decode.rb
require 'rack'

puts Rack::Session::Cookie::Base64::Marshal.new.decode(ARGV[0])
{% endhighlight %}

Running `decode.rb` with the cookie's data results in:

{% highlight bash %}
$ ruby decode.rb BAh7B0kiD3Nlc3Npb25faWQGOgZFRkkiJWYxZGNlMmNmYjFmYjBkODQ0NTc1ZWE3OTBjZmJmNTZkBjsAVEkiDG1lc3NhZ2UGOwBGSSIRSGVsbG8gV29ybGQhBjsARg%3D%3D--decd8e233744e2ba5a80481426e41d72a6685986
{"session_id"=>"f1dce2cfb1fb0d844575ea790cfbf56d", "message"=>"Hello World!"}
{% endhighlight %}

Our message, alongside a generated session identifier, is available *without*
the Rails application's secret session token.

Attempting to tamper with the data, repackage it, and send it back to the
server would fail because the digital signature would not check out, but
**the data is at least readable**.

Rails 4 will feature an encrypted cookie store, where data is both digitally
verified and unreadable to end users. I write about this (and more, of course!)
in [Upgrading to Rails 4](http://www.upgradingtorails4.com/).

In closing, remember:

* The default Rails 3 session store (cookie store) allows users to *read*
the contents of the session. They may not, however, change or tamper with the
session data.
* Do not store data in the session that would be sensitive even if the data
were read by an end user or an attacker sniffing the traffic.
* Switch to the encrypted cookie store when Rails 4 ships.
