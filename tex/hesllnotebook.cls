\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{hesllnotebook}
\LoadClass{article}

%%%%%%%%%%%%%%%%%%%%%%%%%%% 0. some bullshit %%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%% 1. packages required %%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{titlesec}
\RequirePackage{titletoc}
\RequirePackage{graphicx}
\RequirePackage{xcolor}
\RequirePackage{xparse}
\RequirePackage{fancyhdr}
\RequirePackage{minted}
\RequirePackage[utf8]{vntex}
\RequirePackage{fancyhdr}
\RequirePackage{multicol}
\RequirePackage{amsmath}
\RequirePackage{amssymb}
\RequirePackage{enumitem}
\RequirePackage{booktabs}

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
\setcounter{tocdepth}{3}
% compact ToC: footnotesize, no paragraph skip, little space between entries.
% Lengths in pt (em differs between the indent and the footnotesize label): each level's number
% starts where the parent's title starts (16pt, 34pt = 16 + 18).
\titlecontents{section}[16pt]{\addvspace{3pt}\footnotesize\bfseries}{\contentslabel{16pt}}{\hspace*{-16pt}}{\hfill\contentspage}
\titlecontents{subsection}[34pt]{\footnotesize}{\contentslabel{18pt}}{\hspace*{-18pt}}{\titlerule*[0.5pc]{.}\contentspage}
\titlecontents{subsubsection}[57pt]{\footnotesize}{\contentslabel{23pt}}{\hspace*{-23pt}}{\titlerule*[0.5pc]{.}\contentspage}
\let\hesll@tableofcontents\tableofcontents
\renewcommand{\tableofcontents}{{\setlength{\parskip}{0pt}\hesll@tableofcontents}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 5. codes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\setminted{xleftmargin=11pt, resetmargins, numbersep=4pt, breaklines, breakbefore=. , stripnl, linenos, tabsize=2, fontsize=\footnotesize, firstnumber=1}

\def\currentdir{}
% entry heading level: \twolevels (section -> entry) or \threelevels (section -> group -> entry)
\def\entrylevel{subsection}
\newcommand{\twolevels}{\def\entrylevel{subsection}}
\newcommand{\threelevels}{\def\entrylevel{subsubsection}}
% heading for an entry with no content yet
\newcommand{\entry}[1]{\csname\entrylevel\endcsname{#1}}
\newcommand{\setcurrentdir}[1]{\def\currentdir{contents/#1}}
% Text layout (kactl-like). Code notes: one \fld{Label} line per fact, recurrences displayed with \[ \].
\newcommand{\fld}[1]{\par\noindent\hangindent=1em\textbf{#1:}\ }
\newcommand{\tightdisplay}{\setlength\abovedisplayskip{2pt}\setlength\belowdisplayskip{2pt}%
  \setlength\abovedisplayshortskip{0pt}\setlength\belowdisplayshortskip{1pt}}
% Prose: \topic{Heading} per topic, formulas displayed; checklists: \begin{checks}\item ...\end{checks}
\newcommand{\topic}[1]{\par\smallskip\noindent\textbf{#1}\par\nopagebreak}
\newlist{checks}{itemize}{1}
\setlist[checks]{nosep,leftmargin=1.2em,label=$\square$}
\newlist{facts}{itemize}{1}
\setlist[facts]{nosep,leftmargin=1.1em,label=\textbullet}
% \cpp entries show the hash of their printed lines (= contest/hash.sh) at the right of the title
\def\entryhash{}
\ExplSyntaxOn
\NewDocumentCommand\sethash{mmm}{% file, firstline, lastline (empty = to the end)
    \exp_args:Nx\sys_get_shell:nnN{sh~entryhash.sh~#1~#2~#3}{}\l_tmpa_tl
    \tl_gset:Nx\entryhash{\exp_not:N\hfill{\exp_not:N\normalfont\exp_not:N\ttfamily\exp_not:N\scriptsize\l_tmpa_tl}}}
\ExplSyntaxOff
\NewDocumentCommand\othercodes{oommmo}{
    \csname\entrylevel\endcsname[#3]{#3\entryhash}\gdef\entryhash{}
    \IfNoValueTF{#6}{} {%
        {\footnotesize\raggedright\parskip=0pt\tightdisplay #6\par}
        \vspace*{-1em}
    } 
    \IfNoValueTF{#2}{%
        \IfNoValueTF{#1}{\def\hesll@lines{}}{\def\hesll@lines{firstline={#1}}}%
    } {%
        \def\hesll@lines{firstline={#1},lastline={#2}}%
    }%
    % expand the lexer (\cpplexer) first: minted v2 passes it to pygmentize unexpanded
    \expanded{\noexpand\inputminted[\hesll@lines]{#4}}{\currentdir/#5}
}
% minted v3 (2024+) names a custom lexer file.py:Class and needs it enabled in ~/.latexminted_config;
% minted v2 passes the string to pygmentize with -x
\@ifpackagelater{minted}{2024/01/01}
    {\def\cpplexer{customlexers/lexers.py:CustomLexer}}
    {\def\cpplexer{'customlexers/lexers.py -x'}}
\NewDocumentCommand\cpp{oommo}{%
    \sethash{\currentdir/#4}{\IfNoValueTF{#1}{1}{#1}}{\IfNoValueTF{#2}{}{#2}}%
    \othercodes[#1][#2]{#3}{\cpplexer}{#4}[#5]}