#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>
#include "histogramcreator.h"

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(QWidget *parent = 0);
    ~HistogramDialog();

    void setHistogramData(TBLRHistograms hists);

private:
    Ui::HistogramDialog *ui;
};

#endif // HISTOGRAMDIALOG_H
