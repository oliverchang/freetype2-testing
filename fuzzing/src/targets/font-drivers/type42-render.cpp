// type42-render.cpp
//
//   Implementation of Type42RenderFuzzTarget.
//
// Copyright 2018 by
// Armin Hasitzka, David Turner, Robert Wilhelm, and Werner Lemberg.
//
// This file is part of the FreeType project, and may only be used,
// modified, and distributed under the terms of the FreeType project
// license, LICENSE.TXT.  By continuing to use, modify, or distribute
// this file you indicate that you have read the license and
// understand and accept it fully.


#include "targets/font-drivers/type42-render.h"

#include "iterators/faceloaditerator.h"
#include "iterators/faceprepiterator-bitmaps.h"
#include "iterators/faceprepiterator-outlines.h"
#include "visitors/facevisitor-autohinter.h"
#include "visitors/facevisitor-loadglyphs-bitmaps.h"
#include "visitors/facevisitor-loadglyphs-outlines.h"
#include "visitors/facevisitor-subglyphs.h"


  using namespace std;


  const FT_Long  Type42RenderFuzzTarget::NUM_USED_BITMAPS  = 15;
  const FT_Long  Type42RenderFuzzTarget::NUM_USED_OUTLINES =  5;


  Type42RenderFuzzTarget::
  Type42RenderFuzzTarget( void )
  {
    auto  fli = fuzzing::make_unique<FaceLoadIterator>();

    auto  fpi_bitmaps  = fuzzing::make_unique<FacePrepIteratorBitmaps>();
    auto  fpi_outlines = fuzzing::make_unique<FacePrepIteratorOutlines>();


    // -----------------------------------------------------------------------
    // Face preparation iterators:

    (void) fpi_bitmaps
      ->add_visitor(
        fuzzing::make_unique<FaceVisitorLoadGlyphsBitmaps>(
          NUM_USED_BITMAPS ) );

    (void) fpi_outlines
      ->add_visitor( fuzzing::make_unique<FaceVisitorAutohinter>() );
    (void) fpi_outlines
      ->add_visitor(
        fuzzing::make_unique<FaceVisitorLoadGlyphsOutlines>(
          NUM_USED_OUTLINES ) );
    (void) fpi_outlines
      ->add_visitor( fuzzing::make_unique<FaceVisitorSubGlyphs>() );

    // -----------------------------------------------------------------------
    // Face load iterators:

    (void) fli
      ->set_supported_font_format( FaceLoader::FontFormat::TYPE_42 );

    (void) fli->add_iterator( move( fpi_bitmaps  ) );
    (void) fli->add_iterator( move( fpi_outlines ) );

    // -----------------------------------------------------------------------
    // Fuzz target:

    (void) set_iterator( move( fli ) );
  }