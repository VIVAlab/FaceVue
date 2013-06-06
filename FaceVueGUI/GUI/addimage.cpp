#include "gui\addimage.h"

AddImage::AddImage(QWidget *parent)
	: QDialog(parent)
{
    m_lineEdit = new QLineEdit(this);
    m_image = new QLabel(this);
    m_image->setGeometry(10,10,128,128);
    name = new QLabel(tr("Enter Your Name:"));
    comment = new QLabel(tr("Do you want to add the image to your database?"));
    yesButton = new QPushButton(tr("Yes"));
    yesButton->setDefault(true);

    noButton = new QPushButton(tr("No"));

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(yesButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(noButton, QDialogButtonBox::RejectRole);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QHBoxLayout* thl = new QHBoxLayout;
    QVBoxLayout *lt = new QVBoxLayout;
    lt->addWidget(comment);
    QHBoxLayout* hl = new QHBoxLayout;
    hl->addWidget(name);
    hl->addWidget(m_lineEdit);
    lt->addLayout(hl);
    lt->addWidget(buttonBox);
    thl->addWidget(m_image);
    thl->addLayout(lt);
    setLayout(thl);

}
void AddImage::setImage(QImage* image)
{
	m_image->setPixmap(QPixmap::fromImage(*image));
}
AddImage::~AddImage()
{
}
QString AddImage::getName()
{
	return m_lineEdit->text();
}
