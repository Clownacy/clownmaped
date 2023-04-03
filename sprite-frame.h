#ifndef SPRITE_FRAME_H
#define SPRITE_FRAME_H

#include <QVector>

#include "data-stream.h"
#include "sprite-piece.h"
#include "tile-manager.h"

struct SpriteFrame
{
	static SpriteFrame fromDataStream(DataStream &stream);

	void draw(QPainter &painter, const TileManager &tile_manager, Tile::PixmapType effect, int starting_palette_line = 0, int x_offset = 0, int y_offset = 0) const;
	QRect rect() const;
	int left() const {return rect().left();}
	int right() const {return rect().right();}
	int top() const {return rect().top();}
	int bottom() const {return rect().bottom();}

	QVector<SpritePiece> pieces;
};

#endif // SPRITE_FRAME_H
