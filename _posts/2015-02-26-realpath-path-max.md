---
layout: post
title: "realpath and PATH_MAX"
comments: true
date: "2015-02-26T16:00:00Z"
---

The `realpath` function expands a path that potentially contains symbolic links into its canonical name without symbolic links. It is exposed in languages like Ruby via `File.realpath`.

Interestingly, `realpath` can only resolve paths of length `PATH_MAX` bytes or less (e.g., 4096 on most Linux systems I've touched recently). However, according to [this article](http://www.hpenterprisesecurity.com/vulncat/en/vulncat/cpp/often_misused_file_system_realpath.html), **"`PATH_MAX` only bounds the longest possible relative path that can be passed to the kernel in a single call. On most Unix and Linux systems, there is no easily-determined maximum length for a path."**

I decided to prove this was true with a real example.

Consider a bash script that creates a deep directory structure of folders named `ddddddddd{... 255 d's ...}`, nested 17 directories deep. 255 * 17 is 4335 bytes, beyond the 4096 "limit". Along the way the script creates symbolic links of _just one d character_ linked to the _255 d_ version:

{% highlight bash %}
{% include make_links.sh %}
{% endhighlight %}

The result:

{% highlight text %}
{% raw %}
# ls -alhd d
lrwxrwxrwx 1 root root 255 Feb 25 22:21 d -> ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
{% endraw %}
{% endhighlight %}

This directory structure is repeated 17 deep. With its preceding `/`, one link expands to 256 bytes:

{% highlight text %}
{% raw %}
# realpath d | tr -d '\n' | wc -c
256
{% endraw %}
{% endhighlight %}

... two symlinks deep expands to 512:

{% highlight text %}
{% raw %}
# realpath d/d | tr -d '\n' | wc -c
512
{% endraw %}
{% endhighlight %}

... etc ...

The entire directory structure of single lettered directory names expands to a canonical path that's too long for `realpath` to deal with:

{% highlight text %}
{% raw %}
# realpath d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d
d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d: File name too long
{% endraw %}
{% endhighlight %}

There are implications for security if `realpath` is being used to verify that a symbolic link doesn't point to a location outside of a canonical path prefix.

Consider linking `d/d/d{...17 times ...}/hello` to `/tmp/hello`:

{% highlight text %}
{% raw %}
# ln -s /tmp/hello d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d
{% endraw %}
{% endhighlight %}

It is valid to write to this location even though `realpath` cannot determine its canonical path:

{% highlight text %}
{% raw %}
# realpath d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/hello
d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/hello: File name too long
# echo "HI EVERYBODY" > d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/hello
# cat /tmp/hello
HI EVERYBODY
{% endraw %}
{% endhighlight %}

**Be aware of what your language of choice does when the canonical path is too long.**

Ruby raises an exception, which I consider good behavior in this case:

{% highlight text %}
{% raw %}
# ruby -e 'puts File.realpath("d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/hello")'
-e:1:in `realpath': File name too long
{% endraw %}
{% endhighlight %}

Perl, however, returns `undef`. If you're not careful, this might pass as an acceptable value through the system:

{% highlight text %}
{% raw %}
# perl -MCwd -MData::Dumper \
  -e 'print Dumper Cwd::realpath("d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/d/hello")'
$VAR1 = undef;
{% endraw %}
{% endhighlight %}

In Python `os.path.realpath` seems to return 4096 bytes of the canonicalized path but leaves intact any part of the path that could not be expanded beyond 4096 bytes. In that case, symbolic links may still exist in the path returned from `os.path.realpath`, defeating its documented purpose. In Python 3.4, however, pathlib's `Path.resolve` raises an `OSError` (similar to Ruby's behavior).

To wrap up, assume that there is no maximum canonical path length in Linux. Canonical paths can be much longer than `PATH_MAX` bytes even though they can be addressed by paths shorter than `PATH_MAX` via symbolic links. `realpath` will not expand paths beyond `PATH_MAX` bytes, and each language handles it slightly differently.
