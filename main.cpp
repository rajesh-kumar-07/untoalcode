
#include <iostream>
#include <sstream>
#include <vector>
#include <hpdf.h>

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    std::cerr << "ERROR: error_no=" << error_no << ", detail_no=" << detail_no << std::endl;
    exit(1);
}

std::vector<unsigned char> generate_offer_letter_pdf(const std::string &name, const std::string &position) {
    HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
    if (!pdf) {
        throw std::runtime_error("Failed to create PDF object");
    }

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
    HPDF_Page_SetFontAndSize(page, font, 14);

    float height = HPDF_Page_GetHeight(page);
    float width = HPDF_Page_GetWidth(page);

    std::stringstream ss;
    ss << "Dear " << name << ",\n\n"
       << "We are pleased to offer you the position of " << position << " at our company.\n"
       << "Your joining date and other details will be communicated shortly.\n\n"
       << "Best regards,\nHR Department";

    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 50, height - 100);
    HPDF_Page_ShowText(page, ss.str().c_str());
    HPDF_Page_EndText(page);

    // Write PDF to memory stream
    std::vector<unsigned char> buffer;
    HPDF_SaveToStream(pdf);
    HPDF_ResetStream(pdf);
    HPDF_UINT32 size = HPDF_GetStreamSize(pdf);
    buffer.resize(size);
    HPDF_ReadFromStream(pdf, buffer.data(), &size);

    HPDF_Free(pdf);
    return buffer;
}

int main() {
    std::string name = "Rajesh Kumar";
    std::string position = "Embedded Software Intern";

    try {
        std::vector<unsigned char> pdfData = generate_offer_letter_pdf(name, position);

        // Save to file for testing only (remove this part in real deployment)
        FILE *fp = fopen("offer_letter.pdf", "wb");
        fwrite(pdfData.data(), 1, pdfData.size(), fp);
        fclose(fp);

        std::cout << "Offer letter PDF generated in memory (and saved for test)." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
