---
layout: post
title: "My Assets Are No Longer Minified in Rails 4: HALP!"
---

Rails 4.0 and sprockets no longer heed the `config.assets.compress` directive.
Instead, the JavaScript and CSS compressor must be specified explicitly with
`config.assets.js_compressor` and `config.assets.css_compressor`,
respectively.

**No error or deprecation warning is raised: assets will simply not be
minified if the Rails 3 `config.assets.compress = true` directive is left
intact.**

Unminified assets may cause pages to load more sluggishly.

When upgrading a Rails 3 application to Rails 4, make sure to adjust
`config/environments/production.rb` as shown:

{% highlight ruby %}
Widgets::Application.configure do
  # ...

  # Compress JavaScripts and CSS
  # config.assets.compress = true <- remove the Rails 3 setting
  config.assets.js_compressor  = :uglifier
  config.assets.css_compressor = :sass
end
{% endhighlight %}

Both Rails 3 and 4 use `uglifier` and `sass-rails` in the default `Gemfile`, so
these compressors are available unless you have removed them.

With the new settings added, assets will again minify when compiled in
production.

If you are interested in more upgrading steps and checklists, check out the
handbook I am writing: [Upgrading to Rails 4](http://upgradingtorails4.com/).
