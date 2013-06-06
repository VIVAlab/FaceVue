#ifndef ADDIMAGE_H
#define ADDIMAGE_H

#include <QDialog>
#include <qlineedit.h>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qlabel.h>

class AddImage : public QDialog
{
	Q_OBJECT

public:
	explicit AddImage(QWidget *parent = 0);
	~AddImage();
	void setImage(QImage* image);
	QString getName();
private:
	QLineEdit *m_lineEdit;
	QLabel* m_image;
	QLabel* name;
	QLabel* comment;
	QPushButton *yesButton;
	QPushButton *noButton;
	QDialogButtonBox *buttonBox;
	QHBoxLayout* thl;
	QVBoxLayout *lt;
	QHBoxLayout* hl;

};

#endif // ADDIMAGE_H
