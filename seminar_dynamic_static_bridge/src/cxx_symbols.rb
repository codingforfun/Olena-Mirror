class SimpleSymbol
  attr_reader :code
  def initialize ( code )
    @code = code
  end
  def to_s
    @code.to_s
  end
end # class SimpleSymbol

class EncodedSymbol
  attr_reader :name, :code, :cxx_char, :patt
  @@codes     ||= {}
  def initialize ( name, code, cxx_char=nil )
    @name = name
    @code = (code.is_a? Integer)? code.chr.to_sym : code
    @cxx_char = (cxx_char.is_a? Integer)? cxx_char.chr.to_sym : cxx_char
    @patt = to_s.strip
    raise if @@codes[@code]
    @@codes[@code] = self
  end
  def self.[] ( arg )
    @@codes[arg.to_sym]
  end
  def to_s
    (@cxx_char || "#@name ").to_s
  end
  def self.encode ( str )
    # p str
    result = str.gsub '_', '_U_'
    result.gsub!('operator>>', 'operator_RR_')
    result.gsub!('operator<<', 'operator_LL_')
    # result.gsub!(/<\s+</, '<~<')
    # result.gsub!(/>\s+>/, '>~>')
    result.gsub!(/\s*/, '')
    # result.gsub!('~', ' ')
    @@codes.each_value do |v|
      next if [:U, :RR, :LL].include? v.code
      result.gsub!(v.patt, "_#{v.code}_")
    end
    result.gsub!(/\s*/, '')
    result.gsub!(/_+/, '_')
    result.gsub!(/^_/, '')
    result.gsub!(/_$/, '')
    # p result
    result
  end
  def self.enc ( name, code, cxx_char=nil )
    EncodedSymbol.new(name, code, cxx_char)
  end
  enc :underscore, :U, ?_
  enc :slash,      :S, ?/
  enc :dot,        :D, ?.
  enc :function,   :F
  enc :left,       :L, '< '
  enc :right,      :R, ' >'
  enc :leftleft,   :LL, '<<'
  enc :rightright, :RR, '>>'
  enc :ref,        :REF, ?&
  enc :const,      :CONST
  enc :ptr,        :PTR, ?*
  enc :namespace,  :N, '::'
  enc :comma,      :C, ', '
end # class EncodedSymbol

def mangle ( aString )
  EncodedSymbol.encode(aString)
end
