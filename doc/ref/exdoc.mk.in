%.dot: %.file
	cp $$(cat $<) $@

%.texi: %.cmd
## Eval the cmd to preserve embedded redirections.
	eval $$(cat $<) >$*.out 2>$*.err; \
	echo $$? >$*.sta
	rm -f $@
	echo '@example'            >> $@
	echo "$$ @kbd{$$(cat $<)}" >> $@
	if test -s $*.err; then \
	  sed -n -e 's/\([{@}]\)/@\1/g' \
		  -e '/./s/.*/@error{}&/p' $*.err >> $@;\
	fi
	if test -s $*.out; then \
	  expand $*.out | \
	    sed -e 's/\([{@}]\)/@\1/g' \
		-e "s/é/@'e/g" \
		-e 's/è/@`e/g' \
		-e 's/ê/@^e/g' \
		-e 's/ë/@"e/g' \
		-e 's/î/@^i/g' \
		-e 's/ï/@"i/g' \
		-e 's/ô/@^o/g' \
		 >>$@;\
	fi
## Display the exit status only if not 0, as it confuses the
## students to see so many `=> 0'.  But, if there is nothing output
## at all, it is even more confusing, so output `=> 0' only when
## needed.
	if test ! -e $@ \
	   || test $$(cat $*.sta) -ne 0; then \
	  sed 's/.*/@result{}&/' $*.sta >> $@; \
	fi
	echo "@strong{Example $*}: @kbd{$$(cat $<)}" >> $@
	echo '@end example' >> $@

## Once for PDF.
## ratio = 2 makes it wider.
## size (in Inches) makes it fit into A4.
%.eps: %.dot
	dot -Gratio=2 -Gsize=8,8 -Tps2 $*.dot -o $*.ps
	ps2epsi $*.ps $*.eps

%.pdf: %.eps
	epstopdf $*.eps -o $*.pdf

## Another for HTML (JPG), using natural size.
%.jpg: %.dot
	dot -Tps2 $*.dot -o $*.j.ps
	ps2epsi $*.j.ps $*.j.eps
	convert $*.j.eps $*.jpg

## And the text...
%.txt: %.dot
	cp $< $@
