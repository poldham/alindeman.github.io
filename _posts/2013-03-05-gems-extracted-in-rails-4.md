---
layout: post
title: "Upgrading Rails: Gems Extracted in Rails 4"
---

During the development of Rails 4, many features that were present in earlier
versions of Rails were removed from Rails itself and extracted to gems.

Extracting features to gems slims down Rails itself, and allows Rails to take a
different direction to solve certain problems. For example,
[**strong_parameters**](https://github.com/rails/strong_parameters) in Rails 4
is recommended over **attr_accessible** and **attr_protected** from Rails 3
when an application needs to protect itself from mass-assignment
vulnerabilities.

Furthermore, some of the extracted gems have new maintainers. These fresh
maintainers might respond more quickly to bug and feature requests than the
Rails core team feasibly can.

Unfortunately, existing applications that are upgraded to Rails 4 may need to
pull in several new gems in order to perform properly. I have compiled a list
of these extracted gems and the features they provide. If your application uses
any of the listed features, you'll want to pull them into **Gemfile** while
upgrading to Rails 4.

The book I'm writing, [Upgrading to Rails
4](http://www.upgradingtorails4.com/), goes into more detail about these
extractions as well as new features in Rails 4.

<table class="table table-bordered table-condensed">
  <thead>
    <tr>
      <th>Gem</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>
        <a href="https://github.com/rails/protected_attributes">protected_attributes</a>
      </th>
      <td>
        Because Rails 4 recommends <strong>strong_parameters</strong> for
        mass-assignment protection, <strong>attr_accessible</strong> and
        <strong>attr_protected</strong> have been extracted. I expect that most
        upgraded applications will need this gem, as the transition to
        <strong>strong_parameters</strong> can be tedious and error-prone.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/activeresource">activeresource</a>
      </th>
      <td>
        While the ActiveRecord-like abstraction over a RESTful API has always
        shipped as a gem, it is no longer included with Rails by default.
        Include it explicitly if your application requires it.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/actionpack-action_caching">actionpack-action_caching</a><br/>
        <a href="https://github.com/rails/actionpack-page_caching">actionpack-page_caching</a>
      </th>
      <td>
        Rails 4 includes many improvements to fragment caching, but action
        and page caching have been extracted. If your application uses action
        or page caching, be sure to pull these gems in explicitly.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/activerecord-session_store">activerecord-session_store</a>
      </th>
      <td>
        The ability to store session data in a database table has been
        extracted in Rails 4. If your application uses the ActiveRecord
        session store, include this gem.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/rails-observers">rails-observers</a>
      </th>
      <td>
        Rails no longer encourages the use of observers, separate objects
        that can react to lifecycle events of ActiveRecord models. If your
        application uses observers, make sure to include this gem.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/actionpack-xml_parser">actionpack-xml_parser</a>
      </th>
      <td>
        Following security vulnerabilities involving inbound XML, Rails
        extracts the ability to accept XML input to a gem. If your application
        accepts XML in a request body (note: this is distinct from
        <em>responding with</em> XML), you will need to pull in this gem.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/rails/rails-perftest">rails-perftest</a>
      </th>
      <td>
        Rails 4 extracts <strong>ActionDispatch::PerformanceTest</strong>. If
        your application includes performance tests (usually located in
        <strong>test/performance</strong>), add this gem to your bundle.
      </td>
    </tr>
    <tr>
      <th>
        <a href="https://github.com/reed/actionview-encoded_mail_to">actionview-encoded_mail_to</a>
      </th>
      <td>
        Rails previously included a little-known feature to obfuscate email
        addresses in hyperlinks, either with HTML entities or JavaScript
        code. If your application uses the <strong>encode</strong> option with
        <strong>mail_to</strong>, include this gem.
      </td>
    </tr>
  </tbody>
</table>

 If you're interested in more details about upgrading to Rails 4, please
 [consider buying my Upgrading to Rails 4
 handbook](http://www.upgradingtorails4.com/).
