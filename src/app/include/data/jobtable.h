// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_JOBTABLE_H
#define ARRIVAL_JOBTABLE_H

#include <QHash>
#include <QObject>
#include <QList>

#include "data/csvhandling.h"

namespace Arrival::App
{
    class JobTable : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(int newAddedCount READ newAddedCount NOTIFY newAddedCountChanged)
        Q_PROPERTY(int removedCount READ removedCount NOTIFY removedCountChanged)
        Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
        Q_PROPERTY(int columnCount READ columnCount NOTIFY columnCountChanged)
        Q_PROPERTY(bool hasData READ hasData NOTIFY hasDataChanged)
        Q_PROPERTY(QList<QString> headerNames READ headerNames NOTIFY headerNamesChanged)
        Q_PROPERTY(QString formatIdentifier READ formatIdentifier NOTIFY formatIdentifierChanged)
    public:
        /*!
         * \brief JobTable construct a new \c JobTable.
         * \param parent The parent \c QObject.
         */
        explicit JobTable(QObject *parent = nullptr);

        /*!
         * \brief hasData Checks whether the \c JobTable has some actual data.
         * \return True if the \c JobTable has data, false otherwise.
         */
        inline bool hasData() const
        {
            return !(m_data.isNull());
        }

        /*!
         * \brief data Returns a \c QSharedPointer<CSVCombinedData> to the data.
         * \return Pointer to the data.
         */
        QSharedPointer<CSVCombinedData> data()
        {
            return m_data;
        }

        /*!
         * \brief rowCount Returns the row count. If no data is there 0 is returned.
         * \return The row count. If no data is there 0 is returned.
         */
        int rowCount() const
        {
            if (hasData())
            {
                return m_data->rowCount();
            }
            return 0;
        }

        /*!
         * \brief columnCount Returns the column count. If no data is there 0 is returned.
         * \return The column count. If no data is there 0 is returned.
         */
        int columnCount() const
        {
            if (hasData())
            {
                return m_data->columnCount();
            }
            return 0;
        }

        /*!
         * \brief newAddedCount Returns the newAdded count. If no data is there 0 is returned.
         * \return The newAdded count. If no data is there 0 is returned.
         */
        int newAddedCount() const
        {
            if (hasData())
            {
                return m_data->newAddedCount();
            }
            return 0;
        }

        /*!
         * \brief removedCount Returns the removed count. If no data is there 0 is returned.
         * \return The removed count. If no data is there 0 is returned.
         */
        int removedCount() const
        {
            if (hasData())
            {
                return m_data->removedCount();
            }
            return 0;
        }

        /*!
         * \brief headerNames Returns a list of the header names.
         * \return The list of headers.
         */
        QList<QString> headerNames() const
        {
            if (hasData())
            {
                return m_data->headerNames();
            }
            return QList<QString>();
        }

        /*!
         * \brief formatIdentifier Returns the format identifier of the table.
         * \return The identifier.
         */
        const QString formatIdentifier() const
        {
            if (hasData())
            {
                return m_data->formatIdentifier();
            }
            return QString();
        }

    signals:
        /*!
         * \brief preTableReset Called bevore the table reset.
         */
        void preTableReset();

        /*!
         * \brief postTableReset Called after the table reset.
         */
        void postTableReset();

        /*!
         * \brief newAddedCountChanged newAddedCount changed.
         * \param newCount The new count.
         */
        void newAddedCountChanged(int newCount);

        /*!
         * \brief removedCountChanged removedCount changed.
         * \param newCount The new count.
         */
        void removedCountChanged(int newCount);

        /*!
         * \brief rowCountChanged rowCount changed.
         * \param newCount The new rowCount.
         */
        void rowCountChanged(int newCount);

        /*!
         * \brief columnCountChanged columnCount changed.
         * \param newCount The new columnCount.
         */
        void columnCountChanged(int newCount);

        /*!
         * \brief hasDataChanged hasData changed.
         * \param value The new value.
         */
        void hasDataChanged(bool value);

        /*!
         * \brief headerNamesChanged headerNames changed.
         * \param newNames The new value.
         */
        void headerNamesChanged(const QList<QString>& newNames);

        /*!
         * \brief formatIdentifierChanged formatIdentifierChanged changed.
         * \param identifier The new value.
         */
        void formatIdentifierChanged(const QString& identifier);

    public slots:
        /*!
         * \brief setTableData Sets the table data (the underlying pointer).
         * \param data The new data.
         */
        void setTableData(QSharedPointer<CSVCombinedData> data);

        /*!
         * \brief clearTable Clears the table.
         */
        void clearTable();

    private:
        /*!
         * \brief m_data Pointer to the actual data.
         */
        QSharedPointer<CSVCombinedData> m_data;
    };
}

#endif // ARRIVAL_JOBTABLE_H
