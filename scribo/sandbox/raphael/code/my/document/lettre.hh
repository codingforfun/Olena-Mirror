class line
{
  public:
    line(){}
    void add(lettre& ltr)
    {lettres.append(ltr);}
  private:
    mln::util::array<vbbox> lettres;
    std::string name;}
class lettre
{
  public:
    letter(){}
  private:
    std::string name;
    
}