#include "sprite-piece-button.h"

#include <QBrush>
#include <QtGlobal>
#include <QPainter>

#include "utilities.h"

SpritePieceButton::SpritePieceButton(const TileManager &tile_manager)
	: tile_manager(tile_manager)
{
	connect(&tile_manager, &TileManager::regenerated, this, qOverload<>(&SpritePieceButton::update));
}

void SpritePieceButton::setSpritePiece(const SpritePiece &sprite_piece)
{
	const QSizeF dpi_scale = Utilities::GetDPIScale(this);
	m_sprite_piece = sprite_piece;
	setFixedSize(sprite_piece.width * Tile::WIDTH * 1.5 * dpi_scale.width(), sprite_piece.height * Tile::HEIGHT * 1.5 * dpi_scale.height());
	update();
}

void SpritePieceButton::paintEvent(QPaintEvent* const event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.scale(width() / (m_sprite_piece.width * Tile::WIDTH), height() / (m_sprite_piece.height * Tile::HEIGHT));
	m_sprite_piece.draw(painter, tile_manager, Tile::PixmapType::WITH_BACKGROUND);
}
