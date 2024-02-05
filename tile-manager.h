#ifndef TILE_MANAGER_H
#define TILE_MANAGER_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <istream>

#include <QColor>
#include <QObject>
#include <QPixmap>
#include <QVector>

#include "libsonassmd/tile.h"

#include "palette.h"
#include "signal-wrapper.h"

class TileManager : public QObject
{
	Q_OBJECT

public:
	enum class PixmapType
	{
		NO_BACKGROUND,
		WITH_BACKGROUND,
		TRANSPARENT,

		MAX
	};

	static constexpr int TILE_WIDTH = 8;
	static constexpr int TILE_HEIGHT = 8;
	static constexpr int TILE_SIZE_IN_BYTES = TILE_WIDTH * TILE_HEIGHT / 2;

	TileManager(const SignalWrapper<Palette> &palette_manager);

	void unloadTiles()
	{
		tiles.clear();
		tile_pixmaps.clear();

		emit pixmapsChanged();
	}

	bool setTiles(std::istream &stream, const libsonassmd::Game game)
	{
		tiles.clear();

		// TODO: Detect and handle files that end with a partial tile.
		do
		{
			tiles.push_back({}); // TODO: emplace_back
			tiles.back().fromBinaryStream(stream, game);
		} while (!stream.eof());

		tiles.pop_back();

		tile_pixmaps.resize(tiles.size()); // TODO: Reserve instead?

		regeneratePixmaps();

		return true;
	}

	const QPixmap& pixmaps(const int tile_index, const int palette_line, const PixmapType type) const
	{
		if (tile_index >= tile_pixmaps.size())
			return invalid_tile_pixmaps[static_cast<std::size_t>(type)];
		else
			return tile_pixmaps[tile_index][palette_line][static_cast<std::size_t>(type)];
	}

	int total_tiles() const
	{
		return tile_pixmaps.size();
	}

	void deleteTile(int tile_index);
	void duplicateTile(int tile_index, int insert_index);
	void clearTile(int tile_index);
	void modifyTiles(const std::function<void(QVector<libsonassmd::Tile>&)> &callback);
	auto& getTiles() const
	{
		return tiles;
	}

signals:
	void pixmapsChanged();

private slots:
	void regeneratePixmaps();

private:
	void regeneratePixmap(int tile_index);

	std::array<QPixmap, static_cast<std::size_t>(PixmapType::MAX)> createPixmaps(const std::array<std::array<QColor, TILE_WIDTH>, TILE_HEIGHT> rgb_pixels);
	static std::array<std::array<QColor, TILE_WIDTH>, TILE_HEIGHT> createInvalidTilePixmap();

	const Palette &palette;

	std::array<QPixmap, static_cast<std::size_t>(PixmapType::MAX)> invalid_tile_pixmaps;
	QVector<libsonassmd::Tile> tiles;
	QVector<std::array<std::array<QPixmap, static_cast<std::size_t>(PixmapType::MAX)>, Palette::TOTAL_LINES>> tile_pixmaps;
};

#endif // TILE_MANAGER_H
