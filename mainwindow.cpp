#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setModel(&model);

    model.setHorizontalHeaderLabels(QStringList()
                                    << "Build phase"
                                    << "NOK Geometry - Other"
                                    << "NOK Geometry - Missing CAD"
                                    << "NOK Geometry - Feasibility Issue"
                                    << "OK Geometry"
                                    << "Agreed CAD OK %");

    ui->tableView->resizeColumnsToContents();

    ui->tableViewTopIssues->setModel(&topIssuesModel);
    ui->tableViewAllIssues->setModel(&allOtherIssuesModel);

    ui->dateEditBuildStart->setDate(QDate::currentDate());
    ui->dateEditBuildFinish->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButtonBrowseJSON_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), "JSON (*.json)");

    if(!path.isEmpty()) {
        if(!QFile::exists(path)) {return;}

        ui->lineEditPathToJSON->setText(path);

        parseJSONFile();
    }
}

void MainWindow::on_toolButtonAdd_clicked()
{
    model.appendRow(QList<QStandardItem*>()
                    << new QStandardItem(ui->lineEditBuildPhase->text())
                    << new QStandardItem(QString::number(ui->spinBoxGeoRed->value() - ui->spinBoxFeasibilityIssues->value() - ui->spinBoxMissingCad->value()))
                    << new QStandardItem(QString::number(ui->spinBoxMissingCad->value()))
                    << new QStandardItem(QString::number(ui->spinBoxFeasibilityIssues->value()))
                    << new QStandardItem(QString::number(ui->spinBoxGeoGreen->value()))
                    << new QStandardItem(ui->doubleSpinBoxAgreedCadNok->text()));

    ui->tableView->resizeColumnsToContents();
}

void MainWindow::parseJSONFile()
{
    QFile jsonFile(ui->lineEditPathToJSON->text());

    if(!jsonFile.open(QFile::ReadOnly)) {return;}

    QJsonParseError *parseError = new QJsonParseError();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(), parseError);

    if(!jsonDocument.isObject()) {return;}

    QJsonObject jsonObject = QJsonObject::fromVariantMap(jsonDocument.toVariant().value<QVariantMap>());

    //#####################################

    QJsonObject::iterator jsonObjectIterator = jsonObject.find("IssueCategoryDescriptions");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    issueCategoryDescriptions = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("PreAIMSByIssueCategory");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    preAIMSByIssueCategory = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("AIMSByIssueCategory");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    aimsByIssueCategory = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("CoCs");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    cocs = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("PreAIMSSummaryByCoC");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    preAIMSSummaryByCoC = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("AIMSSummaryByCoC");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    aimsSummaryByCoC = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("MissingCAD");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    missingCAD = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("FeasibilityIssues");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    feasibilityIssues = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("NoFeasibilityIssues");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    noFeasibilityIssues = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("PreAIMSByModuleTeam");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    preAIMSByModuleTeam = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("AIMSByModuleTeam");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    aimsByModuleTeam = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("AIMSByModuleTeamAndCOCGrouping");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    aimsByModuleTeamAndCOCGrouping = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("TopIssues");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    topIssues = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("AllOtherIssues");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    allOtherIssues = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("Programme");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    QVariantList programmeVariantList = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("ModelYear");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    QVariantList modelYearVariantList = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("BuildPhase");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    QVariantList buildPhaseVariantList = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################

    jsonObjectIterator = jsonObject.find("NumberOfAIMSRaised");

    if(jsonObjectIterator == jsonObject.end()) {return;}

    QVariantList numberOfAIMSRaisedVariantList = jsonObjectIterator.value().toArray().toVariantList();

    //#####################################
    //#####################################

    if(programmeVariantList.length() != 1 || programmeVariantList[0].toList().length() != 1 ||
            modelYearVariantList.length() != 1 || modelYearVariantList[0].toList().length() != 1 ||
            buildPhaseVariantList.length() != 1 || buildPhaseVariantList[0].toList().length() != 1 ||
            missingCAD.length() != 1 || missingCAD[0].toList().length() != 1 ||
            feasibilityIssues.length() != 1 || feasibilityIssues[0].toList().length() != 1 ||
            noFeasibilityIssues.length() != 1 || noFeasibilityIssues[0].toList().length() != 1 ||
            numberOfAIMSRaisedVariantList.length() != 1 || numberOfAIMSRaisedVariantList[0].toList().length() != 1 ||
            topIssues.length() < 2 ||
            allOtherIssues.length() < 2) {return;}

    programme = programmeVariantList[0].toList()[0].toString();
    modelYear = modelYearVariantList[0].toList()[0].toString();
    buildPhase = buildPhaseVariantList[0].toList()[0].toString();

    numberOfAIMSRaised = numberOfAIMSRaisedVariantList[0].toList()[0].toInt();

    ui->labelBuildEvent->setText(programme + " " +
            modelYear + " " +
            buildPhase);

    ui->labelMissingCAD->setText(missingCAD[0].toList()[0].toString());
    ui->labelFeasibilityIssues->setText(feasibilityIssues[0].toList()[0].toString());
    ui->labelNoFeasibilityIssues->setText(noFeasibilityIssues[0].toList()[0].toString());

    topIssuesModel.clear();
    allOtherIssuesModel.clear();

    topIssuesModel.setHorizontalHeaderLabels(QStringList() << "Image file(s)" << topIssues[0].toStringList());

    for(int index = 1; index < topIssues.length(); index++) {
        QList<QStandardItem*> standardItemRow;

        standardItemRow << new QStandardItem();

        foreach (QVariant cellData, topIssues[index].toList()) {
            QStandardItem *item = new QStandardItem();

            if(cellData.type() == QVariant::Double &&
                    !cellData.toString().contains(QRegularExpression("[^0-9]"))) {
                item->setData(cellData.toLongLong(), Qt::DisplayRole);
            }

            else {
                item->setData(cellData, Qt::DisplayRole);
            }

            standardItemRow << item;
        }

        topIssuesModel.appendRow(standardItemRow);
    }

    allOtherIssuesModel.setHorizontalHeaderLabels(QStringList() << "Image file(s)" << allOtherIssues[0].toStringList());

    for(int index = 1; index < allOtherIssues.length(); index++) {
        QList<QStandardItem*> standardItemRow;

        standardItemRow << new QStandardItem();

        foreach (QVariant cellData, allOtherIssues[index].toList()) {
            QStandardItem *item = new QStandardItem();

            if(cellData.type() == QVariant::Double &&
                    !cellData.toString().contains(QRegularExpression("[^0-9]"))) {
                item->setData(cellData.toLongLong(), Qt::DisplayRole);
            }

            else {
                item->setData(cellData, Qt::DisplayRole);
            }

            standardItemRow << item;
        }

        allOtherIssuesModel.appendRow(standardItemRow);
    }
}

void MainWindow::on_toolButtonUp_clicked()
{
    QList<int> rows;

    foreach (QModelIndex index, ui->tableViewAllIssues->selectionModel()->selectedRows()) {
        int indexRow = index.row();

        if(!rows.contains(indexRow)) {
            rows << indexRow;
        }
    }

    std::sort(rows.begin(), rows.end(), [](int a, int b){return a > b;});

    foreach (int row, rows) {
        topIssuesModel.appendRow(allOtherIssuesModel.takeRow(row));
    }

    ui->tableViewTopIssues->sortByColumn(ui->tableViewTopIssues->horizontalHeader()->sortIndicatorSection(),
                                         ui->tableViewTopIssues->horizontalHeader()->sortIndicatorOrder());
}

void MainWindow::on_toolButtonDown_clicked()
{
    QList<int> rows;

    foreach (QModelIndex index, ui->tableViewTopIssues->selectionModel()->selectedRows()) {
        int indexRow = index.row();

        if(!rows.contains(indexRow)) {
            rows << indexRow;
        }
    }

    std::sort(rows.begin(), rows.end(), [](int a, int b){return a > b;});

    foreach (int row, rows) {
        allOtherIssuesModel.appendRow(topIssuesModel.takeRow(row));
    }

    ui->tableViewAllIssues->sortByColumn(ui->tableViewAllIssues->horizontalHeader()->sortIndicatorSection(),
                                         ui->tableViewAllIssues->horizontalHeader()->sortIndicatorOrder());
}

void MainWindow::on_tableViewTopIssues_customContextMenuRequested(const QPoint &pos)
{
    QTableView *tableView = ui->tableViewTopIssues;

    QModelIndex indexAtPos = tableView->indexAt(pos);

    if(!indexAtPos.isValid()) {return;}

    QModelIndex imagesIndex = topIssuesModel.index(indexAtPos.row(), 0);

    QStringList currentlyAttachedImages = topIssuesModel.data(imagesIndex, Qt::UserRole + 1).toStringList();

    QMenu menu, subMenu;

    subMenu.setTitle("Remove image(s)");
    subMenu.setIcon(QIcon(":/images/removeImage.png"));
    QAction *removeAll = subMenu.addAction(QIcon(":/images/files.png"), "All");

    foreach (QString pathToImage, currentlyAttachedImages) {
        subMenu.addAction(QIcon(":/images/file.png"), pathToImage);
    }

    QAction *addImages = menu.addAction(QIcon(":/images/addImage.png") ,"Add image(s)");

    if(currentlyAttachedImages.count() > 0) {
        menu.addMenu(&subMenu);
    }

    QAction *clickedAction = menu.exec(tableView->viewport()->mapToGlobal(pos));

    if(!clickedAction) {return;}

    if(clickedAction == addImages) {
        QStringList selectedImages = QFileDialog::getOpenFileNames(this, "Select image(s)", QString(), "Images (*.png *.jpg *.bmp)");

        currentlyAttachedImages << selectedImages;
        currentlyAttachedImages.removeDuplicates();

        topIssuesModel.setData(imagesIndex, currentlyAttachedImages, Qt::UserRole + 1);
        topIssuesModel.setData(imagesIndex, currentlyAttachedImages.join("\n"), Qt::DisplayRole);
    }

    else if(clickedAction == removeAll) {
        topIssuesModel.setData(imagesIndex, QStringList(), Qt::UserRole + 1);
        topIssuesModel.setData(imagesIndex, QString(), Qt::DisplayRole);
    }

    else {
        currentlyAttachedImages.removeAt(currentlyAttachedImages.indexOf(clickedAction->text()));

        topIssuesModel.setData(imagesIndex, currentlyAttachedImages, Qt::UserRole + 1);
        topIssuesModel.setData(imagesIndex, currentlyAttachedImages.join("\n"), Qt::DisplayRole);
    }
}

void MainWindow::on_toolButtonFinish_clicked()
{
    QString fileName = ui->labelBuildEvent->text().replace(QRegularExpression("[^0-9a-zA-Z]"), " ").trimmed() + " Virtual Build Feedback.pptx";

    QString reportSavePath = QFileDialog::getSaveFileName(this, "Save report", fileName, "PowerPoint Presentation (*.pptx)");

    if(reportSavePath.isEmpty()) {return;}

    QFile fileToSaveTo(reportSavePath);

    if(fileToSaveTo.exists()) {
        if(!fileToSaveTo.remove()) {
            QMessageBox::warning(this, "Error", "Not possible to overwrite file");
            return;
        }
    }

    if(!QFile::copy(":/template/template.pptx", reportSavePath)) {
        QMessageBox::warning(this, "Error", "Failed to create file");
        return;
    }

    if(!QFile(reportSavePath).setPermissions(QFile::WriteOther)) {
        QMessageBox::warning(this, "Error", "Failed to set 'Write' permissions to file");
        return;
    }

    QScopedPointer<QAxObject> powerPointApplication(new QAxObject());

    if(!powerPointApplication->setControl("Powerpoint.Application")) {
        QMessageBox::warning(this, "Error", "Failed to load PowerPoint.Application");
        return;
    }

    QScopedPointer<QAxObject> presentations(powerPointApplication->querySubObject("Presentations"));

    QScopedPointer<QAxObject> presentation(presentations->querySubObject("Open(QString, bool, bool, bool)", QDir::toNativeSeparators(reportSavePath), false, false, true));

    QScopedPointer<QAxObject> slides(presentation->querySubObject("Slides"));

    int slidesCount = slides->property("Count").toInt();

    for(int slideIndex = 1; slideIndex <= slidesCount; slideIndex++) {
        QScopedPointer<QAxObject> slide(slides->querySubObject("Item(int)", slideIndex));
        QScopedPointer<QAxObject> shapes(slide->querySubObject("Shapes"));

        int shapesCount = shapes->property("Count").toInt();

        QList<QSharedPointer<QAxObject>> shapesToBeDeleted;

        if(slideIndex == 1) {
            QJsonObject jsonObj;
            jsonObj.insert("teamName1", QJsonValue(ui->lineEditTeamName1->text()));
            jsonObj.insert("teamName2", QJsonValue(ui->lineEditTeamName2->text()));
            jsonObj.insert("teamMembers1", QJsonValue(ui->plainTextEditTeamMembers1->toPlainText()));
            jsonObj.insert("teamMembers2", QJsonValue(ui->lineEditTeamName2->text()));

            QJsonArray modelRows;

            for(int rowIndex = 0; rowIndex < model.rowCount(); rowIndex++) {
                QJsonArray row;

                for(int columnIndex = 0; columnIndex < model.columnCount(); columnIndex++) {
                    row.append(QJsonValue::fromVariant(model.data(model.index(rowIndex, columnIndex))));
                }

                modelRows.append(QJsonValue(row));
            }

            jsonObj.insert("processCompat", QJsonValue(modelRows));

            QJsonDocument jsonDocument;
            jsonDocument.setObject(jsonObj);

            QString jsonString(jsonDocument.toJson());

            QScopedPointer<QAxObject> shape(shapes->querySubObject("AddShape(int, double, double, double, double)", 1, 1, 1, 1, 1));
            shape->querySubObject("TextFrame")->querySubObject("TextRange")->setProperty("Text", "{json}" + jsonString);
            shape->dynamicCall("Visible", false);
        }

        for(int shapeIndex = 1; shapeIndex <= shapesCount; shapeIndex++) {
            QSharedPointer<QAxObject> shape(shapes->querySubObject("Item(int)", shapeIndex));

            if(shape->dynamicCall("HasTextFrame").toBool()) {
                QScopedPointer<QAxObject> textFrame(shape->querySubObject("TextFrame"));

                QScopedPointer<QAxObject> textRange(textFrame->querySubObject("TextRange"));

                QString text = textRange->property("Text").toString();

                QString teamName1 = ui->lineEditTeamName1->text().trimmed();
                QString mfgTeamMembers1 = ui->plainTextEditTeamMembers1->toPlainText().trimmed();

                QString teamName2 = ui->lineEditTeamName2->text().trimmed();
                QString mfgTeamMembers2 = ui->plainTextEditTeamMembers2->toPlainText().trimmed();

                if(text.contains("{prg}") && text.contains("{my}") && text.contains("{bp}")) {
                    textRange->dynamicCall("Replace(QString, QString)", "{prg}", programme);
                    textRange->dynamicCall("Replace(QString, QString)", "{my}", modelYear);
                    textRange->dynamicCall("Replace(QString, QString)", "{bp}", buildPhase);
                }

                else if(slideIndex == 1 && text.contains("{firstDay}") && text.contains("{lastDay}")) {
                    textRange->dynamicCall("Replace(QString, QString)", "{firstDay}", ui->dateEditBuildStart->text());
                    textRange->dynamicCall("Replace(QString, QString)", "{lastDay}", ui->dateEditBuildFinish->text());
                }

                else if(slideIndex == 3 && text.contains("{MfgTeam1}") && text.contains("{MfgTeamMembers1}")) {
                    if(!teamName1.isEmpty()) {
                        textRange->dynamicCall("Replace(QString, QString)", "{MfgTeam1}", teamName1);
                    }

                    if(!mfgTeamMembers1.isEmpty()) {
                        textRange->dynamicCall("Replace(QString, QString)", "{MfgTeamMembers1}", mfgTeamMembers1);
                    }

                    if(teamName2.isEmpty() && mfgTeamMembers2.isEmpty()) {


                        shape->setProperty("Width", 24.8 * pointsInCm);
                    }
                }

                else if(slideIndex == 3 && text.contains("{MfgTeam2}") && text.contains("{MfgTeamMembers2}")) {
                    if(teamName2.isEmpty() && mfgTeamMembers2.isEmpty()) {
                        shapesToBeDeleted.append(shape);
                    }

                    if(!teamName2.isEmpty()) {
                        textRange->dynamicCall("Replace(QString, QString)", "{MfgTeam2}", teamName2);
                    }

                    if(!mfgTeamMembers2.isEmpty()) {
                        textRange->dynamicCall("Replace(QString, QString)", "{MfgTeamMembers2}", mfgTeamMembers2);
                    }
                }

                else if(slideIndex == 2 && topIssuesModel.rowCount() == 0 && text.contains("Top Issues")) {
                    textRange->dynamicCall("Replace(QString, QString)", "Top Issues", "");
                }
            }

            else if(shape->dynamicCall("HasTable").toBool()) {
                QScopedPointer<QAxObject> table(shape->querySubObject("Table"));

                if(slideIndex == 1) {
                    QScopedPointer<QAxObject> cell(table->querySubObject("Cell(int, int)", 1, 1));
                    cell->querySubObject("Shape")->querySubObject("TextFrame")->querySubObject("TextRange")->dynamicCall("Replace(QString, QString)", "{bp}", buildPhase);


                    cell.reset(table->querySubObject("Cell(int, int)", 2, 2));
                    cell->querySubObject("Shape")->querySubObject("TextFrame")->querySubObject("TextRange")->dynamicCall("Replace(QString, QString)", "{npr}", ui->spinBoxNumOfProcReviewed->text());


                    cell.reset(table->querySubObject("Cell(int, int)", 3, 2));
                    cell->querySubObject("Shape")->querySubObject("TextFrame")->querySubObject("TextRange")->dynamicCall("Replace(QString, QString)", "{nar}", QString::number(numberOfAIMSRaised));
                }
            }

            else if(shape->dynamicCall("HasChart").toBool()) {
                QScopedPointer<QAxObject> chart(shape->querySubObject("Chart"));

                QScopedPointer<QAxObject> worksheet(chart->querySubObject("ChartData")
                                                    ->querySubObject("Workbook")->querySubObject("Worksheets(int)", 1));

                QScopedPointer<QAxObject> chartDataTable(worksheet->querySubObject("ListObjects(QString)", "Table1"));

                if(slideIndex == 4) {
                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "A2:B100"));
                    range->dynamicCall("ClearContents()");

                    range.reset(worksheet->querySubObject("Range(QString)", "A1"));
                    QString rangeValue = range->property("Value").toString();

                    if(rangeValue == "Issue Category") {
                        int index = 0;

                        for(index = 1; index < issueCategoryDescriptions.count(); index++) {
                            range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(index + 1)));
                            range->setProperty("Value", issueCategoryDescriptions[index].toList()[0].toString());

                            range.reset(worksheet->querySubObject("Range(QString)", "B" + QString::number(index + 1)));
                            range->setProperty("Value", preAIMSByIssueCategory[index].toList()[0].toInt());
                        }

                        range.reset(worksheet->querySubObject("Range(QString)", "A1:B" + QString::number(index < 2 ? 2 : index)));

                        chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                    }

                    else if(rangeValue == "CoC") {
                        int index = 0;

                        for(index = 1; index < cocs.count(); index++) {
                            range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(index + 1)));
                            range->setProperty("Value", cocs[index].toList()[0].toString());

                            range.reset(worksheet->querySubObject("Range(QString)", "B" + QString::number(index + 1)));
                            range->setProperty("Value", preAIMSSummaryByCoC[index].toList()[0].toInt());
                        }

                        range.reset(worksheet->querySubObject("Range(QString)", "A1:B" + QString::number(index < 2 ? 2 : index)));

                        chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                    }
                }

                else if(slideIndex == 5) {
                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "A2:B100"));
                    range->dynamicCall("ClearContents()");

                    range.reset(worksheet->querySubObject("Range(QString)", "A1"));
                    QString rangeValue = range->property("Value").toString();

                    if(rangeValue == "Issue Category") {
                        int index = 0;

                        for(index = 1; index < issueCategoryDescriptions.count(); index++) {
                            range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(index + 1)));
                            range->setProperty("Value", issueCategoryDescriptions[index].toList()[0].toString());

                            range.reset(worksheet->querySubObject("Range(QString)", "B" + QString::number(index + 1)));
                            range->setProperty("Value", aimsByIssueCategory[index].toList()[0].toInt());
                        }

                        range.reset(worksheet->querySubObject("Range(QString)", "A1:B" + QString::number(index < 2 ? 2 : index)));

                        chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                    }

                    else if(rangeValue == "CoC") {
                        int index = 0;

                        for(index = 1; index < cocs.count(); index++) {
                            range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(index + 1)));
                            range->setProperty("Value", cocs[index].toList()[0].toString());

                            range.reset(worksheet->querySubObject("Range(QString)", "B" + QString::number(index + 1)));
                            range->setProperty("Value", aimsSummaryByCoC[index].toList()[0].toInt());
                        }

                        range.reset(worksheet->querySubObject("Range(QString)", "A1:B" + QString::number(index < 2 ? 2 : index)));

                        chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                    }
                }

                else if(slideIndex == 6) {
                    QScopedPointer<QAxObject> range;

                    for(int index = 1; index < cocs.count(); index++) {
                        range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(index + 1)));
                        range->setProperty("Value", cocs[index].toList()[0].toString());

                        range.reset(worksheet->querySubObject("Range(QString)", "D" + QString::number(index + 1)));
                        range->setProperty("Value", aimsSummaryByCoC [index].toList()[0].toString());

                        range.reset(worksheet->querySubObject("Range(QString)", "E" + QString::number(index + 1)));
                        range->setProperty("Value", preAIMSSummaryByCoC[index].toList()[0].toString());
                    }

                    range.reset(worksheet->querySubObject("Range(QString)", "A1:C" + QString::number(cocs.count() < 2 ? 2 : cocs.count())));

                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());

                    range.reset(worksheet->querySubObject("Range(QString)", "A" + QString::number(cocs.count() + 1) + ":D100"));
                    range->dynamicCall("ClearContents()");
                }

                else if(slideIndex == 7) {
                    int table1RowTracker = 3, table2RowTracker = 2, pointTracker = 1;

                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "A2:H100"));
                    range->dynamicCall("ClearContents()");

                    for(int rowIndex = 0; rowIndex < model.rowCount(); rowIndex++) {
                        QString table1RowTrackerString = QString::number(table1RowTracker);

                        range.reset(worksheet->querySubObject("Range(QString)", "A" + table1RowTrackerString));
                        range->setProperty("Value", model.data(model.index(rowIndex, 0)));

                        range.reset(worksheet->querySubObject("Range(QString)", "B" + table1RowTrackerString));
                        range->setProperty("Value", model.data(model.index(rowIndex, 2)).toInt() + model.data(model.index(rowIndex, 3)).toInt());

                        range.reset(worksheet->querySubObject("Range(QString)", "C" + table1RowTrackerString));
                        range->setProperty("Value", model.data(model.index(rowIndex, 1)));

                        range.reset(worksheet->querySubObject("Range(QString)", "D" + table1RowTrackerString));
                        range->setProperty("Value", model.data(model.index(rowIndex, 4)));

                        table1RowTracker += 2;


                        QString table2RowTrackerString = QString::number(table2RowTracker);
                        double cadNOK = model.data(model.index(rowIndex, 5)).toString().replace("%","").toDouble()/100.00;

                        range.reset(worksheet->querySubObject("Range(QString)", "F" + table2RowTrackerString));
                        range->setProperty("Value", pointTracker);

                        range.reset(worksheet->querySubObject("Range(QString)", "G" + table2RowTrackerString));
                        range->setProperty("Value", cadNOK);

                        range.reset(worksheet->querySubObject("Range(QString)", "H" + table2RowTrackerString));
                        range->setProperty("Value", cadNOK);

                        table2RowTracker++;
                        table2RowTrackerString = QString::number(table2RowTracker);

                        pointTracker++;

                        range.reset(worksheet->querySubObject("Range(QString)", "F" + table2RowTrackerString));
                        range->setProperty("Value", pointTracker);

                        range.reset(worksheet->querySubObject("Range(QString)", "G" + table2RowTrackerString));
                        range->setProperty("Value", cadNOK);

                        table2RowTracker++;
                        table2RowTrackerString = QString::number(table2RowTracker);

                        pointTracker++;

                        range.reset(worksheet->querySubObject("Range(QString)", "F" + table2RowTrackerString));
                        range->setProperty("Value", pointTracker);

                        range.reset(worksheet->querySubObject("Range(QString)", "G" + table2RowTrackerString));
                        range->setProperty("Value", cadNOK);

                        table2RowTracker++;
                    }

                    table1RowTracker--;

                    range.reset(worksheet->querySubObject("Range(QString)", "A1:D" + QString::number(table1RowTracker)));
                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());


                    table2RowTracker--;

                    range.reset(worksheet->querySubObject("Range(QString)", "F1:H" + QString::number(table2RowTracker < 2 ? 2 : table2RowTracker)));
                    chartDataTable.reset(worksheet->querySubObject("ListObjects(QString)", "Table2"));
                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                }

                else if(slideIndex == 8) {
                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "B1:Z100"));
                    range->dynamicCall("ClearContents()");

                    range.reset(worksheet->querySubObject("Range(QString)", "A2:Z100"));
                    range->dynamicCall("ClearContents()");

                    int rowIndex = 0, columnIndex = 0;

                    QScopedPointer<QAxObject> cell(worksheet->querySubObject("Cells(int, int)", 2, 2));

                    for(rowIndex = 0; rowIndex < preAIMSByModuleTeam.count(); rowIndex++) {
                        QVariantList row = preAIMSByModuleTeam[rowIndex].toList();

                        for(columnIndex = 0; columnIndex < row.count(); columnIndex++) {
                            cell.reset(worksheet->querySubObject("Cells(int, int)", rowIndex + 1, columnIndex + 1));

                            cell->setProperty("Value", row[columnIndex]);
                        }
                    }

                    range.reset(worksheet->querySubObject("Range(QString)", "A1:" + cell->dynamicCall("Address").toString().replace("$", "")));
                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                }

                else if(slideIndex == 9) {
                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "B1:Z100"));
                    range->dynamicCall("ClearContents()");

                    range.reset(worksheet->querySubObject("Range(QString)", "A2:Z100"));
                    range->dynamicCall("ClearContents()");

                    int rowIndex = 0, columnIndex = 0;

                    QScopedPointer<QAxObject> cell(worksheet->querySubObject("Cells(int, int)", 2, 2));

                    for(rowIndex = 0; rowIndex < aimsByModuleTeam.count(); rowIndex++) {
                        QVariantList row = aimsByModuleTeam[rowIndex].toList();

                        for(columnIndex = 0; columnIndex < row.count(); columnIndex++) {
                            cell.reset(worksheet->querySubObject("Cells(int, int)", rowIndex + 1, columnIndex + 1));

                            cell->setProperty("Value", row[columnIndex]);
                        }
                    }

                    range.reset(worksheet->querySubObject("Range(QString)", "A1:" + cell->dynamicCall("Address").toString().replace("$", "")));
                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());
                }

                else if(slideIndex == 10) {
                    QScopedPointer<QAxObject> range(worksheet->querySubObject("Range(QString)", "B1:Z100"));
                    range->dynamicCall("ClearContents()");

                    range.reset(worksheet->querySubObject("Range(QString)", "A2:Z100"));
                    range->dynamicCall("ClearContents()");

                    int rowTracker = 1, columnIndex = 0;

                    QString lastCoC = "";

                    QList<int> cocShapeCount;
                    QStringList cocShapeNames;

                    QScopedPointer<QAxObject> cell;//(worksheet->querySubObject("Cells(int, int)", 2, 2));

                    for(int rowIndex = 0; rowIndex < aimsByModuleTeamAndCOCGrouping.count(); rowIndex++) {
                        QVariantList row = aimsByModuleTeamAndCOCGrouping[rowIndex].toList();

                        if(rowIndex > 0 && row[0].toString() != lastCoC) {
                            lastCoC = row[0].toString();
                            rowTracker++;

                            cocShapeNames << lastCoC;
                            cocShapeCount << 1;
                        }

                        else if(rowIndex > 0 && row[0].toString() == lastCoC) {
                            cocShapeCount[cocShapeCount.length() - 1] = cocShapeCount.last() + 1;
                        }

                        for(columnIndex = 0; columnIndex < row.count(); columnIndex++) {
                            cell.reset(worksheet->querySubObject("Cells(int, int)", rowTracker, columnIndex + 1));

                            cell->setProperty("Value", row[columnIndex]);
                        }

                        rowTracker++;
                    }

                    cell.reset(worksheet->querySubObject("Cells(int, int)", rowTracker, columnIndex + 1));

                    range.reset(worksheet->querySubObject("Range(QString)", "A1:" + (rowTracker < 2 || columnIndex < 1 ? "B2" : cell->dynamicCall("Address").toString().replace("$", ""))));
                    chartDataTable->dynamicCall("Resize(IDispatch*)", range->asVariant());


                    QScopedPointer<QAxObject> chartShapes(chart->querySubObject("Shapes"));

                    QScopedPointer<QAxObject> plotArea(chart->querySubObject("PlotArea"));
                    double plotAreaLeft = plotArea->property("InsideLeft").toDouble();
                    double plotAreaTop = plotArea->property("InsideTop").toDouble();
                    double plotAreaWidth = plotArea->property("InsideWidth").toDouble();
                    double plotAreaHeight = plotArea->property("InsideHeight").toDouble();

                    int numOfModuleTeams = 0;

                    foreach(int num, cocShapeCount) {
                        numOfModuleTeams += num;
                    }

                    numOfModuleTeams += cocShapeNames.count() + 1;

                    double slotWidth = plotAreaWidth / (double)numOfModuleTeams;

                    double shapeLeft = plotAreaLeft + (slotWidth/2.0) + 5;

                    for(int index = 0; index < cocShapeNames.count(); index++) {
                        double shapeWidth = (slotWidth * (double)cocShapeCount[index]) + slotWidth;

                        QScopedPointer<QAxObject> newShape(chartShapes->querySubObject("AddShape(int, double, double, double, double)", 1, shapeLeft, plotAreaTop, shapeWidth, plotAreaHeight+ 5));

                        QScopedPointer<QAxObject> newShapeTextRange(newShape->querySubObject("TextFrame")->querySubObject("TextRange"));
                        QScopedPointer<QAxObject> newShapeTextRangeFont(newShapeTextRange->querySubObject("Font"));
                        newShapeTextRange->setProperty("Text", cocShapeNames[index]);
                        newShapeTextRangeFont->querySubObject("Color")->setProperty("RGB", QColor(166, 166, 166).name().right(6).toInt(0, 16));
                        newShapeTextRangeFont->setProperty("Size", 8.0);

                        QScopedPointer<QAxObject> newShapeLine(newShape->querySubObject("Line"));
                        newShapeLine->querySubObject("ForeColor")->setProperty("RGB", QColor(191, 191, 191).name().right(6).toInt(0, 16));
                        newShapeLine->setProperty("Weight", 2.0);

                        newShape->querySubObject("Fill")->setProperty("Transparency", 1.0);

                        shapeLeft += shapeWidth;
                    }
                }
            }
        }

        if(slideIndex == 11) {
            int topIssuesModelRowCount = topIssuesModel.rowCount();

            int slideTracker = 11;

            if(topIssuesModelRowCount > 0) {
                for(int index = 1; index < topIssuesModelRowCount; index++) {
                    slide->dynamicCall("Duplicate()");
                }

                double top = -1, left, width;

                QScopedPointer<QAxObject> pageSetup(presentation->querySubObject("PageSetup"));

                double slideWidth = pageSetup->property("SlideWidth").toDouble();
                double slideHeight = pageSetup->property("SlideHeight").toDouble();


                for(int topIssueIndex = 0; topIssueIndex < topIssuesModelRowCount; topIssueIndex++) {
                    QScopedPointer<QAxObject> topIssueSlide(slides->querySubObject("Item(int)", slideTracker));
                    QScopedPointer<QAxObject> topIssueSlideShapes(topIssueSlide->querySubObject("Shapes"));

                    int topIssueSlideShapesCount = topIssueSlideShapes->property("Count").toInt();

                    for(int topIssueSlideShapeIndex = 1; topIssueSlideShapeIndex <= topIssueSlideShapesCount; topIssueSlideShapeIndex++) {
                        QSharedPointer<QAxObject> topIssueSlideShape(topIssueSlideShapes->querySubObject("Item(int)", topIssueSlideShapeIndex));

                        if(topIssueSlideShape->dynamicCall("HasTextFrame").toBool()) {
                            QScopedPointer<QAxObject> textRange(topIssueSlideShape->querySubObject("TextFrame")->querySubObject("TextRange"));

                            QString text = textRange->property("Text").toString();

                            if(text.contains("{aimsNumber}") && text.contains("{originator}") &&
                                    text.contains("{raisedOn}") && text.contains("{severity}")) {
                                textRange->dynamicCall("Replace(QString, QString)", "{aimsNumber}", topIssuesModel.data(topIssuesModel.index(topIssueIndex, 2)).toString());
                                textRange->dynamicCall("Replace(QString, QString)", "{originator}", topIssuesModel.data(topIssuesModel.index(topIssueIndex, 3)).toString());
                                textRange->dynamicCall("Replace(QString, QString)", "{raisedOn}", topIssuesModel.data(topIssuesModel.index(topIssueIndex, 6)).toString());
                                textRange->dynamicCall("Replace(QString, QString)", "{severity}", topIssuesModel.data(topIssuesModel.index(topIssueIndex, 7)).toString());
                            }

                            else if(text.contains("{description}")) {
                                textRange->dynamicCall("Replace(QString, QString)", "{description}", topIssuesModel.data(topIssuesModel.index(topIssueIndex, 9)).toString());
                            }
                        }

                        else {
                            if(top == -1) {
                                top = topIssueSlideShape->property("Top").toDouble();
                                left = topIssueSlideShape->property("Left").toDouble();
                                width = topIssueSlideShape->property("Width").toDouble();
                            }

                            QStringList imagePaths = topIssuesModel.data(topIssuesModel.index(topIssueIndex, 0), Qt::UserRole + 1).toStringList();

                            int imagePathsCount = imagePaths.count();

                            for(int pathIndex = 0; pathIndex < imagePathsCount; pathIndex++) {
                                QString pathToImage = imagePaths[pathIndex];

                                QImage image(pathToImage);

                                double imgLeft, imgTop, imgWidth, imgHeight;

                                if(pathIndex == 0) {
                                    imgLeft = left;
                                    imgTop = top;
                                    imgWidth = width;
                                    imgHeight = imgWidth * (((double)image.height())/((double)image.width()));
                                }

                                else {
                                    imgWidth = 200;
                                    imgHeight = imgWidth * (((double)image.height())/((double)image.width()));

                                    imgTop = (slideHeight / 2.0) + ((imgHeight/4.0) * (pathIndex - 1));
                                    imgLeft = (slideWidth / 2.0) + ((imgWidth/4.0) * (pathIndex - 1));
                                }

                                QScopedPointer<QAxObject> line(topIssueSlideShapes->querySubObject("AddPicture(QString, bool, bool, double, double, double, double)",
                                                                 QDir::toNativeSeparators(pathToImage), false, true,
                                                                 imgLeft, imgTop, imgWidth, imgHeight)->querySubObject("Line"));

                                line->querySubObject("ForeColor")->setProperty("RGB", QColor(Qt::black).name().right(6).toInt(0, 16));
                                line->setProperty("Weight", 0.75);
                            }

                            shapesToBeDeleted << topIssueSlideShape;
                        }
                    }

                    foreach (QSharedPointer<QAxObject> shapeToBeDeleted, shapesToBeDeleted) {
                        shapeToBeDeleted->dynamicCall("Delete()");
                    }

                    shapesToBeDeleted.clear();

                    slideTracker++;
                }
            }

            else {
                slide->dynamicCall("Delete()");
            }
        }

        foreach (QSharedPointer<QAxObject> shapeToBeDeleted, shapesToBeDeleted) {
            shapeToBeDeleted->dynamicCall("Delete()");
        }
    }

    QScopedPointer<QAxObject> slideMasterShapes(presentation->querySubObject("SlideMaster")->querySubObject("Shapes"));
    int slideMasterShapesCount = slideMasterShapes->property("Count").toInt();

    for(int shapeIndex = 1; shapeIndex <= slideMasterShapesCount; shapeIndex++) {
        QScopedPointer<QAxObject> slideMasterShape(slideMasterShapes->querySubObject("Item(int)", shapeIndex));

        if(slideMasterShape->dynamicCall("HasTextFrame").toBool()) {
            QScopedPointer<QAxObject> textRange(slideMasterShape->querySubObject("TextFrame")->querySubObject("TextRange"));

            QString text = textRange->property("Text").toString();

            if(text.contains("{name}")) {
                textRange->dynamicCall("Replace(QString, QString)", "{name}", ui->lineEditName->text());
            }
        }
    }

    presentation->dynamicCall("Save()");
}

void MainWindow::on_toolButtonCADCompatUp_clicked()
{
    auto selectedRows = ui->tableView->selectionModel()->selectedRows();

    if(selectedRows.count() == 0) {return;}

    int firstSelectedRow = selectedRows[0].row();

    int newRow = firstSelectedRow - 1 < 0 ? 0 : firstSelectedRow - 1;

    model.insertRow(newRow, model.takeRow(firstSelectedRow));

    ui->tableView->selectRow(newRow);

}

void MainWindow::on_toolButtonCADCompatDown_clicked()
{
    auto selectedRows = ui->tableView->selectionModel()->selectedRows();

    if(selectedRows.count() == 0) {return;}

    int firstSelectedRow = selectedRows[0].row();

    int newRow = firstSelectedRow + 1 >= model.rowCount() ? firstSelectedRow : firstSelectedRow + 1;

    model.insertRow(newRow, model.takeRow(firstSelectedRow));

    ui->tableView->selectRow(newRow);
}

void MainWindow::on_toolButtonTopIssuesUp_clicked()
{
    auto selectedRows = ui->tableViewTopIssues->selectionModel()->selectedRows();

    if(selectedRows.count() == 0) {return;}

    int firstSelectedRow = selectedRows[0].row();

    int newRow = firstSelectedRow - 1 < 0 ? 0 : firstSelectedRow - 1;

    topIssuesModel.insertRow(newRow, topIssuesModel.takeRow(firstSelectedRow));

    ui->tableViewTopIssues->selectRow(newRow);
}

void MainWindow::on_toolButtonTopIssuesDown_clicked()
{
    auto selectedRows = ui->tableViewTopIssues->selectionModel()->selectedRows();

    if(selectedRows.count() == 0) {return;}

    int firstSelectedRow = selectedRows[0].row();

    int newRow = firstSelectedRow + 1 >= topIssuesModel.rowCount() ? firstSelectedRow : firstSelectedRow + 1;

    topIssuesModel.insertRow(newRow, topIssuesModel.takeRow(firstSelectedRow));

    ui->tableViewTopIssues->selectRow(newRow);
}

void MainWindow::on_toolButtonTake_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if(selectedRows.count() == 0) {return;}

    model.removeRow(selectedRows[0].row());
}

void MainWindow::on_toolButtonBrowsePreviousReport_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), "PowerPoint Presentation (*.pptx)");

    if(!path.isEmpty()) {
        if(!QFile::exists(path)) {return;}

        ui->lineEditPathToPreviousReport->setText(path);

        QScopedPointer<QAxObject> powerPointApplication(new QAxObject());

        if(!powerPointApplication->setControl("Powerpoint.Application")) {
            QMessageBox::warning(this, "Error", "Failed to load PowerPoint.Application");
            return;
        }

        QScopedPointer<QAxObject> presentations(powerPointApplication->querySubObject("Presentations"));

        QScopedPointer<QAxObject> presentation(presentations->querySubObject("Open(QString, bool, bool, bool)", QDir::toNativeSeparators(path), true, true, false));

        QScopedPointer<QAxObject> slides(presentation->querySubObject("Slides"));

        if(slides->property("Count").toInt() > 0) {
            QScopedPointer<QAxObject> slide(slides->querySubObject("Item(int)", 1));

            QScopedPointer<QAxObject> shapes(slide->querySubObject("Shapes"));

            int shapesCount = shapes->property("Count").toInt();

            for(int index = 1; index <= shapesCount; index++) {
                QScopedPointer<QAxObject> shape(shapes->querySubObject("Item(int)", index));

                if(shape->dynamicCall("HasTextFrame").toBool()) {
                    QScopedPointer<QAxObject> textRange(shape->querySubObject("TextFrame")->querySubObject("TextRange"));

                    QString text = textRange->property("Text").toString();

                    if(text.startsWith("{json}")) {
                        QJsonParseError *parseError = new QJsonParseError();

                        QJsonDocument jsonDocument = QJsonDocument::fromJson(text.replace(1, 6, "").toLatin1(), parseError);

                        if(!jsonDocument.isObject()) {return;}

                        QJsonObject jsonObject = QJsonObject::fromVariantMap(jsonDocument.toVariant().value<QVariantMap>());

                        //#####################################

                        QJsonObject::iterator jsonObjectIterator = jsonObject.find("teamName1");

                        if(jsonObjectIterator == jsonObject.end()) {return;}

                        ui->lineEditTeamName1->setText(jsonObjectIterator.value().toString());

                        //#####################################

                        jsonObjectIterator = jsonObject.find("teamName2");

                        if(jsonObjectIterator == jsonObject.end()) {return;}

                        ui->lineEditTeamName2->setText(jsonObjectIterator.value().toString());

                        //#####################################

                        jsonObjectIterator = jsonObject.find("teamMembers1");

                        if(jsonObjectIterator == jsonObject.end()) {return;}

                        ui->plainTextEditTeamMembers1->clear();
                        ui->plainTextEditTeamMembers1->appendPlainText(jsonObjectIterator.value().toString());

                        //#####################################

                        jsonObjectIterator = jsonObject.find("teamMembers2");

                        if(jsonObjectIterator == jsonObject.end()) {return;}

                        ui->plainTextEditTeamMembers2->clear();
                        ui->plainTextEditTeamMembers2->appendPlainText(jsonObjectIterator.value().toString());

                        //#####################################

                        jsonObjectIterator = jsonObject.find("processCompat");

                        if(jsonObjectIterator == jsonObject.end()) {return;}

                        QVariantList modelData = jsonObjectIterator.value().toArray().toVariantList();


                        model.removeRows(0, model.rowCount());

                        foreach (QVariant row, modelData) {
                            QList<QStandardItem*> dataRow;

                            foreach (QVariant cell, row.toList()) {
                                dataRow << new QStandardItem(cell.toString());
                            }

                            model.appendRow(dataRow);
                        }
                    }
                }
            }
        }

        presentation->dynamicCall("Close()");

        if(powerPointApplication->querySubObject("Presentations")->property("Count").toInt() == 0) {
            powerPointApplication->dynamicCall("Quit()");
        }
    }
}
