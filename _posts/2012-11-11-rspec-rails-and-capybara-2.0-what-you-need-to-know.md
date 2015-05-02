---
layout: post
title: "rspec-rails and capybara 2.0: what you need to know"
comments: true
redirect_from: "/2012/11/11/rspec-rails-and-capybara-2.0-what-you-need-to-know.html"
---

### Background

Writing full-stack tests is important. I go into detail about this in my
[Acceptance Testing](http://alindeman.github.com/acceptance_testing/) slide
deck.

In a Rails application, RSpec [request
specs](https://www.relishapp.com/rspec/rspec-rails/docs/request-specs/request-spec)
are often used for the task of writing full-stack tests. In capybara 1.x,
request specs both wrap Rails' `IntegrationTest` framework *and* mix in
[capybara](http://github.com/jnicklas/capybara), a tool for driving web
applications (often via headless web browsers).

### Confusion

The fact that request specs both wrap a Rails `IntegrationTest` *and* have
capybara methods mixed in is confusing. José Valim goes into depth about the
problems in [his blog post entitled "Improving the integration between capybara
and
RSpec"](http://blog.plataformatec.com.br/2012/06/improving-the-integration-between-capybara-and-rspec/).

If you've ever confused `get` (Rails) with `visit` (capybara) or
`response` with `page`, or attempted to explain the difference to a newcomer,
you know it is odd and often frustrating.

The RSpec and capybara teams worked together to alleviate the confusion
in capybara 2.x.

### Changes

Upon upgrading to capybara 2.0, capybara will not be available by default in
RSpec request specs. Instead, a new type of spec--the **[feature
spec](https://www.relishapp.com/rspec/rspec-rails/docs/feature-specs/feature-spec)**--has
been created for use with capybara.

To upgrade to capybara 2.0, you'll need to do a few things:

* Upgrade rspec-rails to 2.12.0 or greater
* Move any tests that use capybara from spec/requests to spec/features.
  Capybara tests use the `visit` method and usually assert against `page`.

Alternatively, you *can* keep using capybara in request specs, but [you'll need
to manually mix in the
methods](https://github.com/rspec/rspec-rails/blob/master/Capybara.md).

### Examples and Recommendations

The rest of this piece has some of my opinions, but opinions I've had good
success with in the applications I develop.

Use RSpec request specs to test interactions with your application as a HTTP
API. To do so, use methods like `get`, `post`, `put`, `delete` and assert
against `response`:

{% highlight ruby %}
# spec/requests/widget_api_spec.rb
describe "widget API" do
  it "allows API clients to create widgets" do
    post widgets_url, widget: { name: "Awesome Widget" }, format: "json"

    expect(response.status).to eq(201) # "Created"
  end
end
{% endhighlight %}

Use RSpec feature specs (with capybara) to test your application as a user
might interact with it. To do so, use methods like `visit` and assert against
`page`. You may also use `feature` instead of `describe` and `scenario`
instead of `it` to increase readability:

{% highlight ruby %}
# spec/features/widget_management_spec.rb
feature "widget management" do
  scenario "creating a new widget" do
    visit root_url
    click_link "New Widget"

    fill_in "Name", with: "Awesome Widget"
    click_button "Create Widget"

    expect(page).to have_text("Widget was successfully created.")
  end
end
{% endhighlight %}

### Further Reading

* [cucumber feature describing feature specs](https://github.com/rspec/rspec-rails/blob/master/features/feature_specs/feature_spec.feature)
* [rspec-rails documentation for upgrading capybara](https://github.com/rspec/rspec-rails/blob/master/Capybara.md)
* [capybara pull request setting :type => :feature](https://github.com/jnicklas/capybara/pull/809)

### Shoutouts

The capybara team, especially [Jonas Nicklas](https://github.com/jnicklas),
were pivotal in making these changes happen. In fact, my own role was small
comparatively. Thank you Jonas!

Thanks to [Darren Coxall](https://twitter.com/freakyDaz), [Jim
Hodgson](https://twitter.com/jimhodgson), and [David
Chelimsky](http://twitter.com/dchelimsky) for reviewing this post.
