// glyphrenderiterator-allmodes.cpp
//
//   Implementation of GlyphRenderIteratorAllModes.
//
// Copyright 2018 by
// Armin Hasitzka, David Turner, Robert Wilhelm, and Werner Lemberg.
//
// This file is part of the FreeType project, and may only be used,
// modified, and distributed under the terms of the FreeType project
// license, LICENSE.TXT.  By continuing to use, modify, or distribute
// this file you indicate that you have read the license and
// understand and accept it fully.


#include "iterators/glyphrenderiterator-allmodes.h"

#include <ft2build.h>
#include FT_GLYPH_H

#include "utils/logging.h"


  void
  GlyphRenderIteratorAllModes::
  run( Unique_FT_Glyph  glyph )
  {
    FT_Error  error;
    
    FT_Glyph         raw_rendered_glyph;
    Unique_FT_Glyph  rendered_glyph = make_unique_glyph();
    Unique_FT_Glyph  buffer_glyph   = make_unique_glyph();
    

    for ( auto  mode = 0; mode < FT_RENDER_MODE_MAX; mode++ )
    {
      error = FT_Glyph_Copy( glyph.get(), &raw_rendered_glyph );
      if ( error != 0 )
      {
        LOG( ERROR ) << "FT_Glyph_Copy failed: " << error;
        break; // we can expect this to fail again; bail out!
      }

      LOG( INFO ) << "render mode: " << mode;

      error = FT_Glyph_To_Bitmap( &raw_rendered_glyph,
                                  static_cast<FT_Render_Mode>( mode ),
                                  0,
                                  1 );

      // Wrap the glyph in a unqiue_ptr asap to reliably invoke the
      // destructor.
      rendered_glyph = make_unique_glyph( raw_rendered_glyph );

      if ( error != 0 )
      {
        LOG( ERROR ) << "FT_Glyph_To_Bitmap failed: " << error;
        continue;
      }

      for ( auto&  visitor : glyph_visitors )
      {
        buffer_glyph = copy_unique_glyph( rendered_glyph );
        if ( buffer_glyph == nullptr )
          break; // we can expect this to fail again; bail out!

        visitor->run( move( buffer_glyph ) );
      }
    }
  }
