[~ AutoGen5 template texi ~]
[~ (use-modules (ice-9 regex))
   (define (psystem . t)
      (let ((cmd (apply string-append t)))
        (display cmd)(display "\n")
        (let ((r (system cmd)))
	   (if (> r 0)
	       (begin
                  (display r)(display "\n")
	          (error "*** system() error"))))))
   (define (texinfo-type-format t)
      (letrec ((fmt (lambda (l)
                      (cond ((null? l) (list #\space))
                            ((eqv? (car l) #\*) (cons #\space l))
                            (#t (cons (car l) (fmt (cdr l))))))))
         (list->string (fmt (string->list t))))) ~][~
   (define (mode-format t)
     (let ((in (or (string-ci=? "IN" t)
                   (string-ci=? "INOUT" t)))
           (out (or (string-ci=? "OUT" t)
                    (string-ci=? "INOUT" t))))
        (sprintf "%s @tab %s" (if in "@sc{in}" "") (if out "@sc{out}" ""))))
   (define (header)
     (let* ((s1 (substring (get "srcfile") (string-length "../../src/")))
	    ;; strip extra leading slashes
	    (s2 (regexp-substitute/global #f "^/*([^/].*)$" s1 1))
	    ;; replace privare extensions (.hxx, .inc) by the public one (.hh)
	    (s3 (regexp-substitute/global #f "\.(hxx|inc)$" s2 'pre ".hh")))
	s3))
   (define (namespace sep)
     (if (exist? "ns")
         (string-append (get "ns") sep)))
   (define (cq-name) ;; namespace qualitified C name, e.g, morpho::erosion
     (string-append (namespace "::") (get "name")))
   (define (tq-name) ;; namespace qualitified Texinfo name, e.g, morpho-erosion
     (cn2tn (cq-name)))
   (define (cn2tn name) ;; C name to Texinfo name.
     ;; This simply does s/::/-/g.
     (regexp-substitute/global #f "::" name 'pre "." 'post))
~]
@menu
[~ FOR processing ~][~ IF (not (exist? "nodoc")) ~]* [~
  (let ((w (get "what"))
        (cn (cq-name))
        (tn (tq-name)))
     (sprintf (if (> (string-length cn) 19)
                  "%s\n                        %s"
                  "%-21s %s") (string-append tn "::") w)) ~]
[~ ENDIF ~][~ ENDFOR ~]@end menu

[~ FOR processing ~][~ IF (not (exist? "nodoc")) ~]

@c ----------------------------------------------------------------------
@node [~ (tq-name) ~]
@unnumberedsubsec [~ (cq-name) ~]
@table @sc
[~ IF what ~]
@item Purpose
[~ what ~][~ ENDIF ~]
@item Prototype

@t{#include "}@i{[~(header)~]}@t{"}
@example
@w{[~ ret ~]}[~ (if (> (string-length (get "ret")) 18) "@*" "") ~] @b{@w{[~ (cq-name) ~][~
  IF (exist? "tpl") ~]<[~
    FOR tpl ", " ~]@w{[~ type ~] [~ name ~]}[~
    ENDFOR tpl ~]>[~
  ENDIF tpl ~]}} ([~
      FOR arg ", " ~][~
        (let* ((type (get "type"))
               (var  (get "name")))
           (string-append "@w{" (texinfo-type-format type) "@var{" var "}}"))
 ~][~ ENDFOR arg ~])
@findex [~ name ~]
@end example

[~ IF (exist? "arg") ~]
@item Parameters
@multitable {XXXXXXXXllllllllll} {IN} {OUT} {XXXXXXXXXXXXXXXXXXXXllllllllllll}
@c @item Parameter @tab IN? @tab OUT? @tab Description
[~    FOR tpl ~]@item @var{[~ name ~]} @tab @tab @tab [~ doc ~]
[~    ENDFOR ~][~    FOR arg ~]@item @var{[~ name ~]} @tab [~
         (mode-format (get "mode")) ~] @tab [~ doc ~]
[~    ENDFOR ~]@end multitable
[~ ENDIF ~]
[~ IF doc ~]
@item Description
[~ doc ~][~ ENDIF ~]
[~ IF FIXME ~]
@item Fixme
[~ FIXME ~][~ ENDIF ~]
[~ IF see ~]
@item See also
[~ FOR see ",@*" ~]@ref{[~ (cn2tn (get "see")) ~]}[~ ENDFOR ~].
[~ ENDIF ~]
[~ IF ex ~]@item Example
@example
[~ (out-push-new ".example") ~][~ ex ~][~ (out-pop) ~][~
   `sed 's/^\\$//' .example` ~]
[~ (out-push-new "example.cc") ~]
#include "basics2d.hh"
#include "[~(header)~]"
using namespace oln;
int main()
{
[~ `sed 's/^\\$//' .example` ~]
}
[~ (out-pop) ~][~
  (define md5sum (shell "md5sum < example.cc"))
  (define binname (string-append md5sum "-a.out"))
  (define exoname (string-append md5sum "-" (get ".exo")))
  (define exiname (get ".exi"))

  (if (> (system (string-append "test -f " exoname ".pdf")) 0)
     (begin
       (psystem "test -f " binname " ||\n"
		"g++ -Wall -W -I ../../src -O2 example.cc -o " binname)
       (if (exist? ".exi")
	  (psystem "ln -sf ../img/" (get "exi")))
       (if (exist? ".exo")
	  (psystem "ln -sf " exoname " " (get "exo")))
       (psystem "./" binname)
       (if (exist? ".exi")
	  (begin
	    (if (> (system (string-append "test -f " exiname ".pdf")) 0)
	       (begin
                  (psystem "convert " exiname " " exiname ".eps")
                  (psystem "convert " exiname " " exiname ".png")
       	          (psystem "epstopdf " exiname ".eps > " exiname ".pdf")))
	    (psystem "rm " exiname)))
       (if (exist? ".exo")
	  (begin
	    (psystem "rm " (get ".exo"))
       	    (psystem "convert " exoname " " exoname ".eps")
       	    (psystem "convert " exoname " " exoname ".png")
       	    (psystem "epstopdf " exoname ".eps > " exoname ".pdf")
))))
~][~ `rm -f .example example.cc` ~]
@end example
@ifnotinfo
@center{[~ IF (exist? ".exi") ~]@image{[~ (begin exiname) ~],30mm,}[~ ENDIF ~] [~
      IF (exist? ".exo") ~]@image{[~ (begin exoname) ~],30mm,}[~ ENDIF ~]}
@end ifnotinfo
[~ ENDIF ~]
@end table
[~ ENDIF nodoc ~][~ ENDFOR processing ~]
