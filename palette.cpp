#include "palette.h"

#include <QFile>

#include "data-stream.h"

Palette::Palette()
{
	for (unsigned int line = 0; line < TOTAL_LINES; ++line)
	{
		for (unsigned int index = 0; index < COLOURS_PER_LINE; ++index)
		{
			const unsigned int intensity_1 = index >= 8 ? ~index & 7 : index;
			// Fun fact: those three bizarre colours at the bottom of SonMapEd's default
			// palette are the result of a bug: one of the colour channels underflow.
			// This bug has been recreated here.
			const unsigned int intensity_2 = index >= 8 ? (~index - 1) & 7 : index;

			unsigned int colour = 0xF000;

			switch (line)
			{
				case 0:
					// This is not what SonMapEd does: SonMapEd just includes a copy of the
					// primary palette line from the selected Sonic game. But I worry that
					// doing that would be a copyright infringement, so instead I use a grey
					// version of the other palette lines here.
					colour |= (intensity_1 << 9) | (intensity_2 << 5) | (intensity_2 << 1);
					break;

				case 1:
					colour |= (intensity_1 << 9) | (intensity_2 << 1);
					break;

				case 2:
					colour |= (intensity_2 << 5) | (intensity_1 << 1);
					break;

				case 3:
					colour |= (intensity_2 << 9) | (intensity_1 << 5);
					break;
			}

			colours[line][index] = MDToQColor(colour);
		}
	}
}

void Palette::loadFromFile(const QString &file_path)
{
	QFile file(file_path);
	if (!file.open(QFile::ReadOnly))
		return;

	DataStream stream(&file);
	stream.setByteOrder(DataStream::BigEndian);

	const std::size_t total_colours = file.size() / 2;

	for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(TOTAL_LINES), total_colours / COLOURS_PER_LINE); ++i)
		for (std::size_t j = 0; j < std::min(static_cast<std::size_t>(COLOURS_PER_LINE), total_colours - i * COLOURS_PER_LINE); ++j)
			colours[i][j] = MDToQColor(stream.read<quint16>());

	emit changed();
}

void Palette::setColour(const unsigned int palette_line, const unsigned int palette_index, const QColor &colour)
{
	if (palette_line >= TOTAL_LINES || palette_index >= COLOURS_PER_LINE)
		return;

	colours[palette_line][palette_index] = MDToQColor(QColorToMD(colour));

	emit changed();
}

QColor Palette::MDToQColor(unsigned int md_colour)
{
	// This isn't exactly what SonMapEd does:
	// For some reason, SonMapEd treats the colours as 4-bit-per-channel,
	// when they're really only 3-bit-per-channel.
	unsigned int red = (md_colour >> 1) & 7;
	unsigned int green = (md_colour >> 5) & 7;
	unsigned int blue = (md_colour >> 9) & 7;

	return QColor(red << 5, green << 5, blue << 5);
}

unsigned int Palette::QColorToMD(const QColor &colour)
{
	unsigned int red = (colour.red() >> 5) & 7;
	unsigned int green = (colour.green() >> 5) & 7;
	unsigned int blue = (colour.blue() >> 5) & 7;

	return (blue << 9) | (green << 5) | (red << 1);
}
