---
layout: post
title: "Porting `iterate` to Ruby"
comments: true
redirect_from: "/2013/06/10/porting-iterate-to-ruby.html"
---

I originally posted this as an email to the [Ruby
Parley](http://rubyrogues.com/parley/) mailing list, but I thought it might
have broader appeal.

I've been writing some code that deals with time ranges recently.

I've also been playing with functional languages where list
manipulation is common when solving pretty much any problem that can be
modeled as list manipulation!

List manipulation is desirable when it results in code that is easier to
reason about, does not mutate state, avoids unnecessary looping structures
(again, often with mutable state), or keeps the door open for solving the
problem in a different way.

While Ruby has `Enumerator` and `Enumerable` (which are great!), they revolve
around methods that yield. But there's a large variety of methods that
"iterate" (without side effects) but don't yield. For instance, ActiveSupport
provides `Time#next_month`, but not `Time#each_month`.

So, I built an enumerator [that is modeled after Clojure's
iterate](http://clojure.github.io/clojure/clojure.core-api.html#clojure.core/iterate).
My `Iterator` assumes you give it a method (free from side effects) that, well,
iterates:

{% highlight ruby %}
class Iterator < Enumerator
  def initialize(object, method)
    super() do |y|
      y << object
      loop { y << object = object.__send__(method) }
    end
  end
end
{% endhighlight %}

The code may be a bit dense at first, but Ruby's `Enumerator` is doing the
heavy lifting. I find it pretty great that Ruby gives me the tools to build
something like that in a few lines of code.

My original use case was dates and times:

{% highlight ruby %}
require 'date'

# First 6 months of a given year
Iterator.new(Date.new(2013, 1, 1), :next_month).take(6)
# [#<Date: 2013-01-01>, #<Date: 2013-02-01>, #<Date: 2013-03-01>,
#  #<Date: 2013-04-01>, #<Date: 2013-05-01>, #<Date: 2013-06-01>]

# With ActiveSupport, range of the first 3 months of a given year
Iterator.new(Time.new(2013, 1, 1), :next_month).
  lazy.map(&:all_month).take(3).to_a
# => [2013-01-01 00:00:00 -0500..2013-01-31 23:59:59 -0500,
#     2013-02-01 00:00:00 -0500..2013-02-28 23:59:59 -0500,
#     2013-03-01 00:00:00 -0500..2013-03-31 23:59:59 -0400]
{% endhighlight %}

`Iterator` also adds an elegant way to generate infinite number sequences which
can often be good substitutes for loops:

{% highlight ruby %}
# First 10 3-digit "palindrome numbers"
Iterator.new(100, :next).
  lazy.select { |n| n.to_s == n.to_s.reverse }.take(10).to_a
# => [101, 111, 121, 131, 141, 151, 161, 171, 181, 191]
{% endhighlight %}

If you're a fan of freedom patching, you can add an #iter\_for method to Object:

{% highlight ruby %}
class Object
  def iter_for(method)
    Iterator.new(self, method)
  end
end
{% endhighlight %}

... then you get:

{% highlight ruby %}
require 'date'

Date.new(2013, 1, 1).iter_for(:next_month).take(6)
# [#<Date: 2013-01-01>, #<Date: 2013-02-01>, #<Date: 2013-03-01>,
#  #<Date: 2013-04-01>, #<Date: 2013-05-01>, #<Date: 2013-06-01>]
{% endhighlight %}

**If you're interested in seeing #iter\_for in Ruby 2.1, please comment on or
watch [Feature Request #8506](https://bugs.ruby-lang.org/issues/8506).**
