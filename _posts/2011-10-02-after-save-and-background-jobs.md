---
title: Callbacks and Background Jobs
layout: post
comments: true
---

**TL;DR**: Don't use `after_{create,update,save}` to enqueue a background
job; instead use `after_commit` because the database transaction will
be committed.

---------

Rails' callbacks are popular way to hook into the lifecycle of a model, and use
these hooks to automatically perform tasks when a model changes.

For instance:

{% highlight ruby %}
class Image < ActiveRecord::Base
  after_create :generate_thumbnail

  def generate_thumbnail
    # ...
  end
end
{% endhighlight %}

In this case, we are generating a thumbnail for an image after it is
created.

Because it's likely that generating a thumbnail is an expensive
operation that could easily bog down the web server and web requests,
many folks would refactor it to enqueue a background job that generates
the thumbnail at some time in the future:

{% highlight ruby %}
class Image < ActiveRecord::Base
  after_create :enqueue_thumbnail_job

  def enqueue_thumbnail_job
    Resque.enqueue(ThumbnailJob, id)
  end
end
{% endhighlight %}

In this case, we are using a [Resque](https://github.com/defunkt/resque)
job to do the heavy lifting. We avoid bogging down web requests with
thumbnail generation.

Great, so imagine the Resque job looks something like:

{% highlight ruby %}
class ThumbnailJob
  def self.perform(id)
    image = Image.find(id)
    image.generate_thumbnail
  end
end
{% endhighlight %}

The problem is that **after_create (and most other Rails callbacks) are
executed within the context of a transaction**: other database sessions
will not see the changes to the database until after the changes are
committed.

If the background worker was incredibly fast at popping off a job, it is
possible that `Image.find(id)` would fail **because the database
transaction had not yet completed**. The row would not be visible to
other database sessions yet.

Granted, it is unlikely that this will occur as there is a delay
between the time a job is enqueued and when it is started by a worker
process; however, it *could* happen in a small number of cases, and
when it does, it would be very hard to debug and reproduce.

So, what's the solution?

The **after_commit** callback, introduced in Rails 3 and [available as a
plugin in Rails 2](https://rubygems.org/gems/after_commit).

{% highlight ruby %}
class Image < ActiveRecord::Base
  after_commit :enqueue_thumbnail_job

  def enqueue_thumbnail_job
    Resque.enqueue(ThumbnailJob, id) unless destroyed?
  end
end
{% endhighlight %}

**NOTE:** We also check to make sure the object is not destroyed, as
`after_commit` is called upon destruction as well (thanks **zimbatm**).

Moral of the story: **if you expect another process (e.g., background
job) to see changes to the database, wait until the database transaction
is completed (after_commit)**.
