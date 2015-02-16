---
layout: post
title: Testing Devise with RSpec Request Specs and Capybara
comments: true
---

Many questions about testing Devise come up regularly in the [freenode/#rspec](irc://irc.freenode.net/rspec) 
IRC channel. Often, folks ask how to use RSpec request specs to write
integration level ("full stack") tests against Devise.

In integration tests, ideally nothing is mocked or stubbed. Using the
[recommended setup for controller
tests](https://github.com/plataformatec/devise/wiki/How-To:-Controllers-and-Views-tests-with-Rails-3-(and-rspec\)),
for instance, is not a good idea in request specs. We want to click
links exactly as a user would, fill in forms exactly how a user would,
etc...

This post assumes you have a Devise setup running already. If not, start
with the [Devise README](https://github.com/plataformatec/devise).

First, RSpec request specs can use either Capybara or Webrat. I prefer
Capybara. To install, add this line to your `Gemfile` and run `bundle
install`:

{% highlight ruby %}
gem 'capybara', :group => :test
{% endhighlight %}

Next, add `require 'capybara/rspec'` to `spec/spec_helper.rb`:

{% highlight ruby %}
ENV["RAILS_ENV"] ||= 'test'
require File.expand_path("../../config/environment", __FILE__)
require 'rspec/rails'
require 'capybara/rspec' ### ADD THIS LINE
{% endhighlight %}

Finally, RSpec request specs must be created in the `spec/requests`
directory under your Rails root, so create that directory if it does not
already exist.

Now you're ready to start writing specs! For example, here's a simple
one I named `spec/requests/user_registration_spec.rb`:

{% highlight ruby %}
require "spec_helper"

describe "user registration" do
  it "allows new users to register with an email address and password" do
    visit "/users/sign_up"

    fill_in "Email",                 :with => "alindeman@example.com"
    fill_in "Password",              :with => "ilovegrapes"
    fill_in "Password confirmation", :with => "ilovegrapes"

    click_button "Sign up"

    page.should have_content("Welcome! You have signed up successfully.")
  end
end
{% endhighlight %}

And here's one to test user sign in, named
`spec/requests/user_sign_in_spec.rb`:

{% highlight ruby %}
require "spec_helper"

describe "user sign in" do
  it "allows users to sign in after they have registered" do
    user = User.create(:email    => "alindeman@example.com",
                       :password => "ilovegrapes")

    visit "/users/sign_in"

    fill_in "Email",    :with => "alindeman@example.com"
    fill_in "Password", :with => "ilovegrapes"

    click_button "Sign in"

    page.should have_content("Signed in successfully.")
  end
end
{% endhighlight %}

Hopefully these serve as a good starting point for your tests.

As you move beyond these basic specs, check out these resources:

* [rspec-rails Request Spec Cukes](https://www.relishapp.com/rspec/rspec-rails/docs/request-specs/request-spec)
* [capybara README](https://github.com/jnicklas/capybara/blob/master/README.rdoc)
* [capybara Node::Actions](http://rubydoc.info/github/jnicklas/capybara/master/Capybara/Node/Actions)
* [capybara Node::Matchers](http://rubydoc.info/github/jnicklas/capybara/master/Capybara/Node/Matchers)
