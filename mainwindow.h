#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include<QTimer>
#include<QPixmap>
#include<windows.h>
#include <QPainter>
#include <QColor>
#include <QString>
#include <vector>
#include "scheduling-algorithms.h"
QT_BEGIN_NAMESPACE
using namespace std;

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct RectangleProperties {
    QColor color;
    QString text;
    int startY;
    int height;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event) override;
public slots:
    void updateTable();
    void createTable(int rows,int cols);
    void readTableData();
    void hideResumeButton();
    void AddRectangles(vector<string>&v);
    void createreadyqeue();
    void updateProcesses(vector<string>&v);

private slots:

    void on_dynamicbutt_clicked();
    void on_stop_or_resume_clicked();
    void enableOrDisablePriority(int index);
    void on_staticbutt_clicked();
    void drawNextRectangle();

private:
    Ui::MainWindow *ui;
    std::vector<RectangleProperties> m_rectangles;
    QTimer *m_timer;
    int m_currentIndex = 0;
    int pre_row=-1;
    int readyindex = 0;
    data_to_output d;
    vector<Process>processes;
    data_to_output data;
    vector<bool>done;
    std::vector<QColor> lightColors {QColor(208, 235, 255),
                                    QColor(165, 216, 255),
                                    QColor(116, 192, 252),
                                    QColor(77, 171, 247),
                                    QColor(51, 154, 240),
                                    QColor(34, 139, 230),
                                    QColor(28, 126, 214),
                                    QColor(25, 113, 194),
                                    QColor(24, 100, 171),
                                    QColor(153, 233, 242),
                                    QColor(102, 217, 232),
                                    QColor(59, 201, 219),
                                    QColor(34, 184, 207),
                                    QColor(21, 170, 191),
                                    QColor(16, 170, 173),
                                    QColor(12, 133, 153),
                                    QColor(11, 114, 133),
                                    QColor(186, 200, 255),
                                    QColor(145, 167, 255),
                                    QColor(116, 143, 252),
                                    QColor(92, 124, 250),
                                    QColor(76, 110, 245),
                                    QColor(66, 99, 235),
                                    QColor(59, 91, 219),
                                    QColor(54, 79, 199)
    };
    std::unordered_map<QString, QColor> colorMap;
};
#endif // MAINWINDOW_H
