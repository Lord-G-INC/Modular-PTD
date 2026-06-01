#include "TextWriter.h"

namespace BlueCoinUtil {
    extern u32 getTotalBlueCoinNumCurrentFile(bool);

}
TextWriter::TextWriter(const char* pName) : NameObj(pName) {}

void TextWriter::init(const JMapInfoIter& rIter) {
    OSReport("Amongu s\n");
    //MR::connectToScene(this, 0x21, 5, 9, 0x3C);
    MR::connectToSceneLayout(this);
}

void TextWriter::draw() const {
    //MR::setupDrawForNW4RLayout(1.0f);
    //nw4r::ut::WideTextWriter writer;
    //writer.SetCursor(0.0f, 0.0f);
    //writer.SetFont(*MR::getMenuFontNW4R());
    //writer.SetFontSize(25, 25);
    //writer.SetLineSpace(3);
    //writer.SetCharSpace(0);
    //writer.SetWidthLimit(1000.0f);
    //writer.SetDrawFlag(1);
//
    //Mtx mtx;
    //PSMTXIdentity(mtx);
    //mtx[0][3] = -180.0f;
    //mtx[1][1] = -1.0f;
    //mtx[1][3] = 125.0f;
    //mtx[2][3] = 0.0f;
//
    //GXLoadPosMtxImm(mtx, GX_PNMTX0);
    //GXSetCurrentMtx(GX_PNMTX0);
//
    //nw4r::ut::Color topCol(0xFF0000FF);
    //nw4r::ut::Color btmCol(0xFFFFFFFF);
    //writer.SetGradationMode(nw4r::ut::CharWriter::GRADMODE_NONE);
    //writer.SetTextColor(topCol/*, btmCol*/);
    //nw4r::ut::Color minCol(0x0);
    //nw4r::ut::Color maxCol(0xFFFFFFFF);
    //writer.SetColorMapping(minCol, maxCol);
    //writer.SetupGX();
//
    //f32 v = writer.Printf(L"starshine dead hack HAHHAAAAHAHAHAHAHAHAHHAH", BlueCoinUtil::getTotalBlueCoinNumCurrentFile(false));
}

TextWriter::~TextWriter() {}