class Vcs

  def oln_commit! ( *args )

    common_commit!("proto-1.0 <%= rev %>: <%= title %>", *args) do |subject|
      mail!(:to => ['olena-patches@lrde.epita.fr'], :subject => subject)
    end

  end
  alias_command :olnci, :oln_commit

end # class Vcs
