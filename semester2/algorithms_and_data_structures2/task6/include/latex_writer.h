#ifndef LATEX_WRITER_H
#define LATEX_WRITER_H

#include <fstream>
#include <string>

class latex_writer final
{
private:
    
    static latex_writer* _instance;
   
    std::ofstream _out_file;

    latex_writer() = default;

public:
   
    latex_writer(
        const latex_writer&) = delete;
    
    latex_writer& operator=(
        const latex_writer&) = delete;
   
    ~latex_writer() noexcept;

public:
 
    static latex_writer& instance();

    void write(
        const std::string& text);
   
    void write_line(
        const std::string& text);

    void begin_math();
   
    void end_math();
   
    void begin_align();
   
    void end_align();
   
    void begin_equation();
   
    void end_equation();
   
    void begin_matrix();
   
    void end_matrix();

    void write_section(
        const std::string& title);
   
    void write_subsection(
        const std::string& title);
   
    void new_page();
   
    void add_spacing();

    bool init(
        const std::string& filename);
  
    bool is_open() const;
  
    void close();
};

#endif //LATEX_WRITER_H