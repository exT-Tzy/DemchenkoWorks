#include "latex_writer.h"

latex_writer* latex_writer::_instance = nullptr;

latex_writer::~latex_writer()
{
    close();
}

latex_writer& latex_writer::instance()
{
    if (_instance == nullptr)
    {
        _instance = new latex_writer();
    }

    return *_instance;
}

bool latex_writer::init(
    const std::string& filename)
{
    if (_out_file.is_open())
    {
        _out_file.close();
    }

    _out_file.open(filename, std::ios::out | std::ios::trunc);
    
    if (!_out_file.is_open())
    {
        return false;
    }

    _out_file << R"(\documentclass[12pt,a4paper]{article}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage[english,russian]{babel}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{amsthm}
\usepackage{amsfonts}
\usepackage{mathtools}
\usepackage{geometry}
\usepackage{fancyhdr}
\usepackage{graphicx}
\usepackage{color}
\usepackage{hyperref}

\geometry{margin=2cm}
\pagestyle{fancy}
\fancyhf{}
\fancyhead[C]{Linear Algebra Problem Solutions}
\fancyfoot[C]{\thepage}

\title{Linear Algebra Problem Solutions}
\author{Mathematical Problem Solver}
\date{\today}

\begin{document}
\maketitle
\tableofcontents
\newpage

)";
   
    return true;
}

void latex_writer::close()
{
    if (_out_file.is_open())
    {
        _out_file << "\n\\end{document}\n";
        _out_file.close();
    }
}

void latex_writer::write(
    const std::string& text)
{
    if (_out_file.is_open())
    {
        _out_file << text;
    }
}

void latex_writer::write_line(
    const std::string& text)
{
    if (_out_file.is_open())
    {
        _out_file << text << "\n";
    }
}

void latex_writer::begin_math()
{
    write_line("\\[");
}

void latex_writer::end_math()
{
    write_line("\\]");
}

void latex_writer::begin_align()
{
    write_line("\\begin{align*}");
}

void latex_writer::end_align()
{
    write_line("\\end{align*}");
}

void latex_writer::begin_equation()
{
    write_line("\\begin{equation}");
}

void latex_writer::end_equation()
{
    write_line("\\end{equation}");
}

void latex_writer::begin_matrix()
{
    write_line("\\begin{pmatrix}");
}

void latex_writer::end_matrix()
{
    write_line("\\end{pmatrix}");
}

void latex_writer::write_section(
    const std::string& title)
{
    write_line("\\section{" + title + "}");
}

void latex_writer::write_subsection(
    const std::string& title)
{
    write_line("\\subsection{" + title + "}");
}

void latex_writer::new_page()
{
    write_line("\\newpage");
}

void latex_writer::add_spacing()
{
    write_line("\\vspace{0.5cm}");
}

bool latex_writer::is_open() const
{
    return _out_file.is_open();
}