class Vcs
  # See http://rubyforge.org/projects/vcs
  # and http://vcs.rubyforge.org

  protocol_version '0.1'

  def olena_commit! ( *args )

    common_commit!("proto-1.0 <%= rev %>: <%= title %>", *args) do |subject|
      mail!(:to => ['olena-patches@lrde.epita.fr'], :subject => subject)
    end

  end
  alias_command  :olnci, :olena_commit
  default_commit :olena_commit

end # class Vcs
