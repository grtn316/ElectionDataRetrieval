#include <wx/grid.h>

class CustomFloatRenderer : public wxGridCellFloatRenderer
{
public:
    CustomFloatRenderer(int width, int precision)
        : wxGridCellFloatRenderer(precision)
    {
        SetWidth(width);
    }

    void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc,
        const wxRect& rect, int row, int col,
        bool isSelected) override
    {
        double value;
        wxString text = grid.GetCellValue(row, col);
        text.ToDouble(&value);
        text = wxString::Format("%.2f", value);
        wxGridCellFloatRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
    }
};
