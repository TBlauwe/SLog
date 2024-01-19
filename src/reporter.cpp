#include <slog/reporter.hpp>

slog::NumericalConsoleReporter::NumericalConsoleReporter(const char * title)
{
    // Header
    fmt::format_to(std::back_inserter(out), fmt::emphasis::bold,
                   "┌{0:─^{2}}┐\n│{1: ^{2}}│\n└{0:─^{2}}┘\n", "", title, 80);

    // Columns' names
    fmt::format_to(std::back_inserter(out), label_format, "");
    fmt::format_to(std::back_inserter(out), column_format, "Begin");
    fmt::format_to(std::back_inserter(out), column_format, " End ");
    fmt::format_to(std::back_inserter(out), "Percentage\n");
}

void slog::NumericalConsoleReporter::add_line(const char *name, float begin, float end)
{
    float percentage = begin != 0.f ? (end - begin) / begin * 100 : 0.f;
    auto background = line_count % 2 ? fmt::bg(odd_line_color) : fmt::bg(even_line_color);

    fmt::format_to(std::back_inserter(out), background, label_format, name);
    fmt::format_to(std::back_inserter(out), fmt::emphasis::italic | background, column_format,
                   begin);
    fmt::format_to(std::back_inserter(out), fmt::emphasis::italic | background, column_format, end);

    fmt::text_style percentage_style{};
    if (percentage > 0)
        percentage_style = fmt::fg(fmt::rgb{232, 80, 69});
    else if (percentage < 0)
        percentage_style = fmt::fg(fmt::rgb{69, 232, 83});

    fmt::format_to(std::back_inserter(out), background | percentage_style, "{: 2f}%", percentage);
    fmt::format_to(std::back_inserter(out), background, "{: ^17}", "");
    fmt::format_to(std::back_inserter(out), "\n"); // Prevent background from bleeding

    line_count++;
}

void slog::NumericalConsoleReporter::print()
{
    fmt::format_to(std::back_inserter(out), fmt::emphasis::bold, " {:─^80} ", "");
    fmt::print("{}\n", std::string{out.data(), out.size()});
}
