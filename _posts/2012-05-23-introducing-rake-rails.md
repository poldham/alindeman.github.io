---
layout: post
title: "Introducing rake-rails"
---
[JRubyConf](http://jrubyconf.com) 2012 was a blast. Thanks
[Nick](http://blog.nicksieger.com/) and all the others who made it happen.

I enjoyed many of the talks, including [Adventures on the Golden
Path](https://speakerdeck.com/u/j3/p/adventures-on-the-golden-path) by [Jeff
Casimir](http://twitter.com/#!/j3).

Admittedly, I do not agree with all of his ideas or his implementations, but
one that struck me as a good idea **and low hanging fruit** was the confusion
among the `rails` and `rake` commands:

<script async class="speakerdeck-embed" data-slide="39" data-id="4f710744a1d1bd002200abc1" data-ratio="1.299492385786802" src="//speakerdeck.com/assets/embed.js"></script>

**Beginners and advanced users get tripped up by this one all the time.**

So, I tried whipping up a gem that delegates `rake generate`, `rake console`,
etc.. to their respective `rails` command.

It's called **rake-rails**:

{% highlight ruby %}
# Gemfile
gem 'rake-rails'
{% endhighlight %}

Profit! You can now run ALL the things with `rake`.

{% highlight bash %}
$ rake generate migration add_name_to_users name:string
$ rake console staging
# ...
{% endhighlight %}

For efficiency freaks: it's still pretty fast and does **not** involve loading
the Rails environment multiple times; it simply delegates to `rails/cli` as
quickly as possible, just like the `rails` command does.

Try it out and let me know what you think! Everything is up on
[GitHub](https://github.com/alindeman/rake-rails).
