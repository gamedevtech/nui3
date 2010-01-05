/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLText.h"

///////////////////////////////////////// nuiHTMLText
nuiHTMLText::nuiHTMLText(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor, const nglString& rText)
: nuiHTMLItem(pNode, pAnchor, true), mText(rText), mpLayout(NULL), mpFont(NULL)
{

}

nuiHTMLText::~nuiHTMLText()
{
  if (mpFont)
    mpFont->Release();
  delete mpLayout;
}

void nuiHTMLText::Draw(nuiDrawContext* pContext)
{
  pContext->SetTextColor(mTextFgColor);
  //nuiColor mTextBgColor;
  pContext->SetFont(mpFont, false);
  pContext->DrawText(0, mpLayout->GetAscender() , *mpLayout);
}

void nuiHTMLText::Layout(nuiHTMLContext& rContext)
{
  delete mpLayout;
  if (mpFont)
    mpFont->Release();
  mpFont = rContext.mpFont;
  mpFont->Acquire();
  
  mpLayout = new nuiFontLayout(*mpFont, 0, 0, nuiHorizontal);
  mpLayout->SetUnderline(rContext.mUnderline);
  mpLayout->SetStrikeThrough(rContext.mStrikeThrough);
  mpLayout->Layout(mText);
  mIdealRect = mpLayout->GetRect();
  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;
  //printf("text layout done (%ls)\n", mIdealRect.GetValue().GetChars());
}

float nuiHTMLText::GetAscender() const
{
  return mpLayout->GetAscender();
}

float nuiHTMLText::GetDescender() const
{
  return mpLayout->GetDescender();
}


