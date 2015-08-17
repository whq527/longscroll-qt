#include "imageinfowidget.h"
#include "ui_imageinfowidget.h"
#include <QImageReader>
#include <QFileInfo>

ImageInfoWidget::ImageInfoWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageInfoWidget)
{
	ui->setupUi(this);
}

ImageInfoWidget::ImageInfoWidget(const ContentItemInfo & info, QWidget * parent)
    : ImageInfoWidget(parent)
{
	setItemInfo(info);
}

ImageInfoWidget::~ImageInfoWidget()
{
	delete ui;
}

// inspred by http://permalink.gmane.org/gmane.comp.lib.qt.general/34969
static QString fileSizeDisplayString(qint64 size)
{
	QStringList units;
	units << "KiB" << "MiB" << "GiB" << "TiB" << "PiB" << "EiB" << "ZiB" << "YiB";

	qreal displaySize = qreal(size);
	QString displayUnit = "bytes";
	for (QString const & unit : units)
	{
		if (displaySize < 1024.0)
			break;
		displaySize /= 1024.0;
		displayUnit = unit;
	}
	return QString("%1 %2").arg(displaySize, 0, 'f' , 2).arg(displayUnit);
}

void ImageInfoWidget::setItemInfo(const ContentItemInfo & info)
{
	QFileInfo fi(info.data.toString());
	QString const & fileName = fi.absoluteFilePath();
	QImageReader ir(fileName);

	ui->nameLabel->setText(fileName);
	ui->fileSizeLabel->setText(fileSizeDisplayString(fi.size()));
	ui->fileSizeLabel->setToolTip(QString("%1 bytes").arg(fi.size()));
	ui->sizeLabel->setText(QString("%1x%2 px").arg(ir.size().width()).arg(ir.size().height()));
	ui->qualityLabel->setText(tr("Quality: %1").arg(ir.quality()));
	ui->qualityLabel->setVisible(ir.quality() >= 0);

#if QT_VERSION >= 0x050400
	QString const & subtype = ir.subType();
	if (!subtype.isEmpty())
		ui->fileFormatLabel->setText(QString("%1/%2").arg(ir.format(), subtype));
	else
#endif
		ui->fileFormatLabel->setText(ir.format());

	QImage const & img = ir.read();
	ui->imageWidget->setPixmap(QPixmap::fromImage(img));
	ui->errorLabel->setText(ir.errorString());
	ui->errorLabel->setVisible(img.isNull());
}
