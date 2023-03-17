#ifndef SPRITE_PIECE_H
#define SPRITE_PIECE_H

#include <QPainter>

#include "data-stream.h"
#include "tile-manager.h"

struct SpritePiece
{
	static SpritePiece fromDataStream(DataStream &stream);

	void draw(QPainter &painter, const TileManager &tile_manager, int x_offset = 0, int y_offset = 0) const;

	int x;
	int y;
	int width;
	int height;
	bool priority;
	int palette_line;
	bool y_flip;
	bool x_flip;
	int tile_index;
};

#endif // SPRITE_PIECE_H