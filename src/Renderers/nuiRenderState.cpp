/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nuiRenderState::nuiRenderState()
: mpShaderState(NULL)
{
  mBlending = false;
  mTexturing = false;
  mColorBuffer = true;
  mDepthTest = false;
  mDepthWrite = true;
  mCulling = false;
  mCullingMode = eCullingBack;
  mClearDepth = 1.0f;

  mLineCap = nuiLineCapBut;
  mLineJoin = nuiLineJoinMiter;
  mMitterLimit = 0.5;

  mBlendFunc = nuiBlendTransp;

  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
    mpTexture[i] = NULL;
  mpShader = NULL;

  mAntialiasing = false;
  mWinding = nuiShape::eNonZero;
  mLineWidth = 1.0f;

  mpFont = NULL;

}

nuiRenderState::nuiRenderState(const nuiRenderState& rState)
: mpShaderState(NULL)
{
  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
    mpTexture[i] = NULL;
  mpShader = NULL;
  mpFont = NULL;

  Copy(rState);
}

nuiRenderState& nuiRenderState::operator =(const nuiRenderState& rState)
{
  Copy(rState);
  return *this;
}

void nuiRenderState::Copy(const nuiRenderState& rState)
{
  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
  {
    nuiTexture* pTexture1 = rState.mpTexture[i];
    nuiTexture* pTexture2 = mpTexture[i];
    if (pTexture1)
      pTexture1->Acquire();
    if (pTexture2)
      pTexture2->Release();
  }

  if (rState.mpShader)
    rState.mpShader->Acquire();
  if (mpShader)
    mpShader->Release();

  if (rState.mpShaderState)
    rState.mpShaderState->Acquire();
  if (mpShaderState)
    mpShaderState->Release();

  if (rState.mpFont)
    rState.mpFont->Acquire();
  if (mpFont)
    mpFont->Release();

  memcpy(&mpShader, &rState.mpShader, offsetof(nuiRenderState, mMitterLimit) - offsetof(nuiRenderState, mpShader) + sizeof(mMitterLimit) );
  
#if 0
  mBlending       = rState.mBlending;
  mTexturing      = rState.mTexturing;
  mColorBuffer    = rState.mColorBuffer;
  mDepthTest      = rState.mDepthTest;
  mDepthWrite     = rState.mDepthWrite;
  mWinding        = rState.mWinding;
  mAntialiasing   = rState.mAntialiasing;
  mLineWidth      = rState.mLineWidth;
  mBlendFunc      = rState.mBlendFunc;
  mTextColor      = rState.mTextColor;
  mClearColor     = rState.mClearColor;
  mStrokeColor    = rState.mStrokeColor;
  mFillColor      = rState.mFillColor;
  mLineCap        = rState.mLineCap;
  mLineJoin       = rState.mLineJoin;
  mMitterLimit    = rState.mMitterLimit;
  mCulling        = rState.mCulling;
  mCullingMode    = rState.mCullingMode;
  mClearDepth     = rState.mClearDepth;


  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
  {
    nuiTexture* pOldTexture = mpTexture[i];
    mpTexture[i] = rState.mpTexture[i];
    if (mpTexture[i])
      mpTexture[i]->Acquire();
    if (pOldTexture)
      pOldTexture->Release();
  }

  nuiShaderProgram* pOldShader = mpShader;
  mpShader = rState.mpShader;
  if (mpShader)
    mpShader->Acquire();
  if (pOldShader)
    pOldShader->Release();

  if (rState.mpShaderState)
    rState.mpShaderState->Acquire();
  if (mpShaderState)
    mpShaderState->Release();
  mpShaderState = rState.mpShaderState;

  nuiFont* pOldFont = mpFont;
  mpFont = rState.mpFont;
  if (mpFont)
    mpFont->Acquire();
  if (pOldFont)
    pOldFont->Release();
#endif

}

nuiRenderState::~nuiRenderState()
{
  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
  {
    if (mpTexture[i])
      mpTexture[i]->Release();
  }

  if (mpShader)
    mpShader->Release();

  if (mpShaderState)
    mpShaderState->Release();

  if (mpFont)
    mpFont->Release();
}

bool nuiRenderState::operator==(const nuiRenderState& rState) const
{
  bool state =
    (mBlending       == rState.mBlending)         &&
    (mTexturing      == rState.mTexturing)        &&
    (mColorBuffer    == rState.mColorBuffer)      &&
    (mDepthTest      == rState.mDepthTest)        &&
    (mDepthWrite     == rState.mDepthWrite)       &&
    (mWinding        == rState.mWinding)          &&
    (mAntialiasing   == rState.mAntialiasing)     &&
    (mLineWidth      == rState.mLineWidth)        &&
    (mBlendFunc      == rState.mBlendFunc)        &&
    (mTextColor      == rState.mTextColor)        &&
    (mClearColor     == rState.mClearColor)       &&
    (mStrokeColor    == rState.mStrokeColor)      &&
    (mFillColor      == rState.mFillColor)        &&
    (mLineCap        == rState.mLineCap)          &&
    (mLineJoin       == rState.mLineJoin)         &&
    (mMitterLimit    == rState.mMitterLimit)      &&
    (mpShader        == rState.mpShader)          &&
    (mpShaderState   == rState.mpShaderState)     &&
    (mCulling        == rState.mCulling)          &&
    (mCullingMode    == rState.mCullingMode)      &&
    (mClearDepth     == rState.mClearDepth)       &&
    (mpFont          == rState.mpFont);
  for (int i = 0; i < NUI_MAX_TEXTURE_UNITS; i++)
    state = state && (mpTexture[i] == rState.mpTexture[i]);

  return state;
}
