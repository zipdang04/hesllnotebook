\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{hesllnotebook}
\LoadClass{article}

%%%%%%%%%%%%%%%%%%%%%%%%%%% 0. some bullshit %%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%% 1. packages required %%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{titlesec}
\RequirePackage{graphicx}
\RequirePackage{xcolor}
\RequirePackage{xparse}
\RequirePackage{fancyhdr}
\RequirePackage{minted}
\RequirePackage[utf8]{vntex}
\RequirePackage{fancyhdr}
\RequirePackage{multicol}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 2. margins %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\paperwidth=29.7cm
\paperheight=21cm
\textwidth=28cm %-1cm
\textheight=19.3cm

\hoffset=-3.8cm % one inch + \hoffset
\voffset=-2.04cm % one inch + \voffset
\topmargin=0cm
\headheight=0.5cm
\headsep=0.2cm
% \footskip=0pt

\parindent=0mm
\parskip=1ex
\columnsep=10pt
\columnseprule=1pt
\renewcommand\baselinestretch{1}

% \newlength{\hesllHUGE} 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 3. titles %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% first page
\def\teamname{Team Name\ }
\def\teamuni{University Name}
\def\teamlogo{Team Logo Path}

\newcommand{\setteamname}[1]{\def\teamname{#1\ }}
\newcommand{\setteamuni}[1]{\def\teamuni{#1\ }}
\newcommand{\setteamlogo}[1]{\def\teamlogo{#1}}
\renewcommand{\maketitle}{
    \pagestyle{empty}
    \begin{center}
        \vspace*{\fill}
        \includegraphics[width=10cm]{\teamlogo}
        
        % {\fontsize{2\HUGE}{2.2\HUGE}\selectfont \teamname\\} 
        % \vspace{0.5cm}
        % {\fontsize{\HUGE}{\HUGE}\selectfont \teamuni\\} 
        {\fontsize{65.69pt}{72.26pt}\selectfont \textbf{\teamname}\\} 
        \vspace{0.5cm}
        {\fontsize{32.845pt}{36.130pt}\selectfont \teamuni\\} 
        \vspace*{\fill}
    \end{center}
    \setcounter{page}{0}
    \clearpage
    \pagestyle{fancy}
    \lhead{\teamname -- \teamuni}
    \rhead{page \thepage}
    \cfoot{}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 4. section %%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
\titlespacing*{\section} {0pt}{1ex}{0.75ex}
\titlespacing*{\subsection} {0pt}{0.75ex}{0.5ex}
\titlespacing*{\subsubsection}{0pt}{0.5ex}{0.25ex}
\setcounter{tocdepth}{2}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 3. codes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\setminted{xleftmargin=11pt, resetmargins, numbersep=4pt, breaklines, breakbefore=. , stripnl, linenos, tabsize=2, fontsize=\footnotesize, firstnumber=1}

\def\currentdir{}
\newcommand{\setcurrentdir}[1]{\def\currentdir{contents/#1}}
\NewDocumentCommand\othercodes{oommm}{
    \subsubsection{#3}\addcontentsline{toc}{subsubsection}{#3} 
    \IfNoValueTF{#2}{%
        \IfNoValueTF{#1}{%
            \inputminted{#4}{\currentdir/#5}
        } {%
            \inputminted[firstline={#1}]{#4}{\currentdir/#5}
        }
    } {%
        \inputminted[firstline={#1},lastline={#2}]{#4}{\currentdir/#5}
    }
}
\NewDocumentCommand\cpp{oomm}{\othercodes[#1][#2]{#3}{'customlexers/lexers.py -x'}{#4}}