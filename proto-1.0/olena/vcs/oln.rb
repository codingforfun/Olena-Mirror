class Vcs

  def oln_commit! ( s, *args )

    common_commit!(*args) do |rev|
      news!(:groups => ['lrde.olena.patches'],
            :subject => "proto-1.0 #{rev}: #{s}")
    end

  end
  alias_command :olnci, :oln_commit

end # class Vcs
