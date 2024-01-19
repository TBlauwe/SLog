/*****************************************************************//**
 * @file   reporter.hpp
 * @brief  Header file - Defines additional way to print information.
 * 
 * @author TBlauwe
 * @date   January 2024
 *********************************************************************/

#include <fmt/color.h>
#include <fmt/format.h>


namespace slog
{
    class NumericalConsoleReporter
    {
      public:
        NumericalConsoleReporter(const char *title);

        void add_line(const char *name, float begin, float end);
        void print();

      private:
        unsigned int line_count{0};
        fmt::memory_buffer out;

        // Style
        static constexpr const char *label_format{"{:>30} "};
        static constexpr const char *column_format{"{:^12}"};
        static constexpr fmt::rgb even_line_color{fmt::rgb(20, 20, 20)};
        static constexpr fmt::rgb odd_line_color{fmt::rgb(40, 40, 40)};
    };
} // namespace slog
