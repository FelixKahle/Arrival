// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_JOBLISTMODEL_H
#define ARRIVAL_JOBLISTMODEL_H

#include <QString>
#include <QAbstractTableModel>

#include "data/jobtable.h"

namespace Arrival::App
{
    /*!
     * \brief The JobTableModel class the data model behind a \c JobList
     */
    class JobTableModel : public QAbstractTableModel
    {
        Q_OBJECT

        Q_PROPERTY(JobTable* jobTable READ jobTable WRITE setJobTable NOTIFY jobTableChanged)
        Q_PROPERTY(QList<int> columnsToShow READ columnsToShow WRITE setColumnsToShow NOTIFY columnsToShowChanged)
    public:
        Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Q_INVOKABLE int columns() const;
        Q_INVOKABLE JobTableRowState::State rowState(int index) const;

    public:

        /*!
         * \brief JobTableModel Constructor.
         * \param parent The parent \c QObject
         */
        explicit JobTableModel(QObject *parent = nullptr);

        /*!
         * \brief columnCount The column count.
         * \return The column count.
         */
        int columnCount(const QModelIndex & = QModelIndex()) const override;

        /*!
         * \brief rowCount The row count.
         * \return The column count.
         */
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        /*!
         * \brief data Returns the data.
         * \param index index of the data.
         * \param role Role.
         * \return The data.
         */
        QVariant data(const QModelIndex& index, int role) const override;

        /*!
         * \brief mapColumnIndex Maps the column index.
         * Qt starts at index 0, however we might want to hide this column.
         * So it is necessary to map the index Qt asks for to the index we want to show.
         * \param index The index to map.
         * \return The mapped index.
         */
        int mapColumnIndex(int index) const;

        /*!
         * \brief roleNames Return the role names.
         * \return The role names inside a \c QHash
         */
        QHash<int, QByteArray> roleNames() const override;

        /*!
         * \brief jobTable Returns the \c JobTable.
         * \return The \c JobTable.
         */
        JobTable* jobTable() const;

        /*!
         * \brief columnsToShow Returns a list containg all the indices that should be shown in the table.
         * \return The indices that should be visible in the table.
         */
        const QList<int>& columnsToShow() const;

    signals:
        /*!
         * \brief jobTableChanged Called when the JobTable of this model changed.
         * \param table The new \c JobTable.
         */
        void jobTableChanged(JobTable* table);

        /*!
         * \brief columnsToShowChanged Called when the \c QList<int> of indices of columns that should
         * be visible changes.
         * \param columnsToShow The new \c QList<int> of indices of columns that should be visible.
         */
        void columnsToShowChanged(const QList<int>& columnsToShow);

    public slots:
        /*!
         * \brief setJobTable Sets the \c JobTable of this model.
         * \param list The \c JobTable of this model.
         */
        void setJobTable(JobTable* table);

        /*!
         * \brief setColumnsToShow Sets the \c QList<int> of indices of columns that should be visible.
         * \param columnsToShow The new \c QList<int> of indices of columns that should be visible.
         */
        void setColumnsToShow(const QList<int>& columnsToShow);

    private:
        /*!
         * \brief m_columnsToShow Indices of columns that should be visible.
         */
        QList<int> m_columnsToShow;

        /*!
         * \brief m_table Data of the model.
         */
        JobTable* m_table;
    };
}

#endif // ARRIVAL_JOBLISTMODEL_H
