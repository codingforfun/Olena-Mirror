[~ AutoGen5 template tex ~]
[~

  DEFINE PRINTPROTOTYPE

~]\texttt{{[~ (cn2tex (get "ret")) ~]}[~ (if (> (string-length (get "ret")) 18) "\\\\\n" "") ~]} \mbox{\textbf{[~ (cn2tex (cq-name)) ~]}[~
  IF (exist? "tpl") ~]texttt{<[~
    FOR tpl ", " ~]{[~ type ~] [~ name ~]}[~
    ENDFOR tpl ~]>}[~
  ENDIF tpl ~]} \texttt{([~
      FOR arg ", " ~][~
        (let* ((type (get "type"))
               (var  (cn2tex (get "name"))))
           (string-append "\\mbox{" (cn2tex (texinfo-type-format type)) "\\var{" var "}}"))
 ~][~ ENDFOR arg ~]);}
[~ ENDDEF
# ----------------------------------------------------------------------
~][~

   DEFINE PRINTPROTOTYPES

~][~ FOR ns ~][~ PRINTPROTOTYPE ~]
[~ ENDFOR ns ~][~ ENDDEF
# ----------------------------------------------------------------------
~][~
   (use-modules (ice-9 regex))

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
         (list->string (fmt (string->list t)))))

   (define (mode-format t)
     (let ((in (or (string-ci=? "IN" t)
                   (string-ci=? "INOUT" t)))
           (out (or (string-ci=? "OUT" t)
                    (string-ci=? "INOUT" t))))
        (sprintf "%s & %s" (if in "\\textsc{in}" "") (if out "\\textsc{out}" ""))))

   (define (header)
     (let* ((s1 (get "srcfile")) ;; (substring (get "srcfile") (string-length "../../doc/")))
	    ;; strip extra leading slashes
	    ;; (s2 (regexp-substitute/global #f "^/*([^/].*)$" s1 1))
	    (s2 (regexp-substitute/global #f "^.*/oln(/.*)$" s1 1 'post))
	    ;; replace privare extensions (.hxx, .inc) by the public one (.hh)
	    (s3 (regexp-substitute/global #f "\.(hxx|inc)$" s2 'pre ".hh")))
	s3))

   (define (namespace sep)
     (if (exist? "ns")
         (string-append (get "ns") sep)))

   (define (cq-name) ;; namespace qualified C name, e.g, morpho::erosion
     (string-append (namespace "::") (get "name")))

   (define (cn2tex name) ;; C name to Tex name.
     ;; This simply does s/_/\_/g.
     (let ((x (regexp-substitute/global #f "([_&])" name 'pre "\\" 1 'post)))
        (regexp-substitute/global #f "([<>]+)" x 'pre "$" 1 "$" 'post)))

   (define (tex-name)
     (cn2tex (cq-name)))

;; ----------------------------------------------------------------------
~]
[~ FOR processing ~][~ IF (not (exist? "nodoc")) ~]

% ----------------------------------------------------------------------
\subsection{[~ (tex-name) ~]}\label{[~ (cq-name) ~]}
\begin{description}
[~ IF what ~]
\item[\textsc{Purpose}]~\\
[~ (cn2tex (get "what")) ~]
[~ ENDIF ~]
\item[\textsc{Prototype}]~\\
\texttt{\#include <oln/}\textit{[~(cn2tex (header))~]}\texttt{>}
\begin{rawhtml}
<PRE>
</PRE>
\end{rawhtml}
\begin{flushleft}
[~ PRINTPROTOTYPES ~][~ (define processing.name (get "name"))
~][~ FOR processingoverload ~][~ IF (equal? processing.name (get "name"))
~][~ PRINTPROTOTYPES ~][~ ENDIF ~][~ ENDFOR processing ~]
\end{flushleft}

[~ IF (exist? "arg") ~]
\item[\textsc{Parameters}]~\\
\begin{tabular}[t]{rccl}
[~    FOR tpl ~]\var{[~ (cn2tex (get "name")) ~]} & & & [~ (cn2tex (get "doc")) ~] \\
[~    ENDFOR ~][~
      FOR arg ~]\var{[~ (cn2tex (get "name")) ~]} & [~
         (mode-format (get "mode")) ~] & [~ (cn2tex (get "doc")) ~] \\
[~    ENDFOR ~]\end{tabular}
%[~ ENDIF ~]
[~ IF doc ~]
\item[\textsc{Description}]~\\
[~ (cn2tex (get "doc")) ~][~ ENDIF ~]
[~ IF FIXME ~]
\item[\textsc{Fixme}]~\\
[~ FIXME ~][~ ENDIF ~]
[~ IF see ~]
\item[\textsc{See also}]~\\
[~ FOR see ",\\\\\n" ~][~ (cn2tex (get "see")) ~], \S\ref{[~ see ~]}\begin{latexonly}, p.\pageref{[~ see ~]}\end{latexonly}[~ ENDFOR ~].
[~ ENDIF ~]
[~ IF ex ~]\item[\textsc{Example}]\begin{latexonly}~\\[-2em]\end{latexonly}
\begin{verbatim}
[~ (out-push-new ".example") ~][~ ex ~][~ (out-pop) ~][~
   `sed 's/^\\$//' .example` ~]
[~ IF (not (exist? "wontcompile")) ~][~ (out-push-new "example.cc") ~]
#include <oln/basics2d.hh>
#include <oln/[~(header)~]>
[~ FOR exh ~]
#include <oln/[~ exh ~]>
[~ ENDFOR exh ~]

using namespace oln;
int main()
{
[~ `sed 's/^\\$//' .example` ~]
}
[~ (out-pop) ~][~
  (define md5sum (shell "md5sum < example.cc | cut -f 1 -d ' '"))
  (define binname (string-append md5sum "-a.out"))
  (define b/binname (string-append "bin/" binname))
  (define exoname (string-append md5sum "-" (get ".exo")))
  (define b/exoname (string-append "bin/" exoname))
  (define exiname (get ".exi"))
  (define b/exiname (string-append "bin/" exiname))
  (define (basename filename . ext)
      (eval `(regexp-substitute/global #f "\\..+$" ,filename 'pre ,@ext)))
  (define (eps filename) (basename filename ".eps"))
  (define (pdf filename) (basename filename ".pdf"))
  (define (png filename) (basename filename ".png"))
  (define (b/eps filename) (string-append "bin/" (eps filename)))
  (define (b/pdf filename) (string-append "bin/" (pdf filename)))
  (define (b/png filename) (string-append "bin/" (png filename)))
  (define (h/png filename) (string-append "html/" (png filename)))

  (if (> (system (string-append "test -f " (pdf b/exoname))) 0)
     (begin
       (psystem "test -f " b/binname " ||\n"
		"{ mkdir -p bin ; $CXX -Wall -W -I$OLN_INCLUDEDIR -O2 example.cc -o " b/binname " ; }")
       (if (exist? ".exi")
	  (psystem "cp -f $IMGDIR/" exiname " " b/exiname))
       (if (exist? ".exo")
	  (psystem "ln -sf " exoname " bin/" (get "exo")))
       (psystem "cd bin && ./" binname)
       (if (exist? ".exi")
	  (begin
	    (if (> (system (string-append "test -f " (pdf b/exiname))) 0)
	       (begin
		  (psystem "convert " b/exiname " " (eps b/exiname))
      	          (psystem "mkdir -p html ; convert " b/exiname " " (h/png exiname))
      	          (psystem "epstopdf " (eps b/exiname) " > " (pdf b/exiname))
	        ))
	    (psystem "rm " b/exiname)))
       (if (exist? ".exo")
	  (begin
	    (psystem "rm bin/" (get ".exo"))
	    (psystem "convert " b/exoname " " (eps b/exoname))
            (psystem "mkdir -p html ; convert " b/exoname " " (h/png exoname))
       	    (psystem "epstopdf " (eps b/exoname) " > " (pdf b/exoname))
))))
~][~ ENDIF ~][~ `rm -f .example example.cc` ~]
\end{verbatim}[~
  IF (not (exist? "wontcompile")) ~][~
  IF (or (exist? ".exi") (exist? ".exo"))
~]\begin{latexonly}
\begin{figure}[!h] % Apparently `!h' is not enough to force the figure at
                   % the current position :(
[~ IF (exist? ".exi") ~]
\begin{minipage}[b]{.46\linewidth}
\centering\includegraphics[width=30mm]{bin/[~ (basename exiname) ~]}
\caption{[~ (begin exiname) ~]}
\end{minipage}[~
  ENDIF ~][~ IF (exist? ".exo") ~]
\hfill
\begin{minipage}[b]{.46\linewidth}
\centering\includegraphics[width=30mm]{bin/[~ (basename exoname) ~]}
\caption{[~ (get ".exo") ~]}
\end{minipage}
[~ ENDIF ~]
\end{figure}
\end{latexonly}
\begin{rawhtml}
<TABLE>
<TR>
<TD>
<IMG SRC="[~ (png exiname) ~]"><BR>
[~ (begin exiname) ~]
</TD>
<TD>
<IMG SRC="[~ (png exoname) ~]"><BR>
[~ (get ".exo") ~]
</TD>
</TABLE>
\end{rawhtml}
[~ ENDIF ~][~ ENDIF ~][~ ENDIF ~]
\end{description}
[~ ENDIF nodoc ~][~ ENDFOR processing ~]
