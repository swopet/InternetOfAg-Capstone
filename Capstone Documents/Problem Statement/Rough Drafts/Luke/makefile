capstone: 
	latex capstone.tex
	latex capstone.tex
	dvipdf capstone.dvi
	make clean	
clean:
	rm -f *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.toc

cleanpdf:
	rm -f *.pdf

default: 
	make capstone
	make clean
