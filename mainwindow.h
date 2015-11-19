#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAxObject>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QStandardItemModel>
#include <QVariant>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButtonBrowseJSON_clicked();

    void on_toolButtonAdd_clicked();

    void on_toolButtonUp_clicked();

    void on_toolButtonDown_clicked();

    void on_tableViewTopIssues_customContextMenuRequested(const QPoint &pos);

    void on_toolButtonFinish_clicked();

    void on_toolButtonCADCompatUp_clicked();

    void on_toolButtonCADCompatDown_clicked();

    void on_toolButtonTopIssuesUp_clicked();

    void on_toolButtonTopIssuesDown_clicked();

    void on_toolButtonTake_clicked();

    void on_toolButtonBrowsePreviousReport_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel model, topIssuesModel, allOtherIssuesModel;

    const double pointsInCm = 28.3464567;

    QVariantList issueCategoryDescriptions,
    preAIMSByIssueCategory,
    aimsByIssueCategory,
    cocs,
    preAIMSSummaryByCoC,
    aimsSummaryByCoC,
    missingCAD,
    feasibilityIssues,
    noFeasibilityIssues,
    preAIMSByModuleTeam,
    aimsByModuleTeam,
    aimsByModuleTeamAndCOCGrouping,
    topIssues,
    allOtherIssues;

    QString programme, modelYear, buildPhase;

    int numberOfAIMSRaised;

    void parseJSONFile();
};

#endif // MAINWINDOW_H
