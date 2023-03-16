#include "main-window.h"
#include "./ui_main-window.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget* const parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
    , tile_pixmaps(&tiles_bytes, &palette)
	, sprite_viewer(tile_pixmaps, sprite_mappings)
    , palette_editor(palette)
    , tile_viewer(tile_pixmaps)
{
	ui->setupUi(this);

	auto horizontal_box = new QHBoxLayout();

	horizontal_box->addWidget(&palette_editor);
	horizontal_box->addWidget(&sprite_viewer);

	auto vertical_box = new QVBoxLayout();

	vertical_box->addLayout(horizontal_box);
//	vbox->addsp
	vertical_box->addWidget(&tile_viewer);

	centralWidget()->setLayout(vertical_box);

	sprite_viewer.setBackgroundColour(palette.getColourQColor(0, 0));

	connect(ui->actionOpen_Tiles, &QAction::triggered, this,
		[this]()
		{
			const QString file_path = QFileDialog::getOpenFileName(this, "Open Tile File");

			if (!file_path.isNull())
			{
				QFile file(file_path);
				if (!file.open(QFile::ReadOnly))
					return;

				QDataStream in_stream(&file);

				tiles_bytes.resize(file.size());

				in_stream.readRawData(reinterpret_cast<char*>(tiles_bytes.data()), tiles_bytes.size());

				tile_pixmaps.setTiles(tiles_bytes);
			}
		}
	);

	connect(ui->actionOpen, &QAction::triggered, this,
		[this]()
		{
			const QString file_path = QFileDialog::getOpenFileName(this, "Open Palette File");

			if (!file_path.isNull())
			{
				palette.loadFromFile(file_path);
				tile_pixmaps.setPalette(palette);
			}
		}
	);

	connect(ui->actionOpen_Mappings, &QAction::triggered, this,
		[this]()
		{
			const QString file_path = QFileDialog::getOpenFileName(this, "Open Mappings File");

			if (!file_path.isNull())
				sprite_mappings.loadFromFile(file_path);
		}
	);

	connect(&palette, &Palette::singleColourChanged, &tile_pixmaps, &TilePixmaps::regenerate);

	connect(&palette, &Palette::singleColourChanged, this,
		[this](const unsigned int palette_line, const unsigned int palette_index, const QColor &colour)
		{
			if (palette_line == 0 && palette_index == 0)
				sprite_viewer.setBackgroundColour(colour);
		}
	);
	connect(&palette, &Palette::allColoursChanged, this,
		[this]()
		{
			sprite_viewer.setBackgroundColour(palette.getColourQColor(0, 0));
		}
	);

	connect(&tile_pixmaps, &TilePixmaps::regenerated, &tile_viewer, &TileViewer::refresh);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* const event)
{
	switch (event->key())
	{
		case Qt::Key::Key_BracketLeft:
			sprite_viewer.selectPreviousSprite();
			break;

		case Qt::Key::Key_BracketRight:
			sprite_viewer.selectNextSprite();
			break;
	}
}
