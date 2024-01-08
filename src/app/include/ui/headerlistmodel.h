// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_HEADERLISTMODEL_H
#define ARRIVAL_HEADERLISTMODEL_H

#include <QAbstractListModel>

#include "data/jobtable.h"

#define ARRIVAL_HEADERLISTMODEL_DEFAULT_SELECT_VALUE false

namespace Arrival::App
{
    class HeaderListModel : public QAbstractListModel
    {
        Q_OBJECT

        Q_PROPERTY(JobTable* jobTable READ jobTable WRITE setJobTable NOTIFY jobTableChanged)
        Q_PROPERTY(QList<bool> selectedHeaders READ selectedHeaders WRITE setSelectedHeaders NOTIFY selectedHeadersChanged)
        Q_PROPERTY(QList<int> selectedHeaderIndices READ selectedHeaderIndices WRITE setSelectedHeaderIndices NOTIFY selectedHeaderIndicesChanged)
    public:

        /*!
         * \brief HeaderListModel Constructor.
         * \param parent The parent \c QObject.
         */
        explicit HeaderListModel(QObject *parent = nullptr);

        /*!
         * \brief The RoleNames enum Specifies the roles of this \c QAbstractListModel
         */
        enum RoleNames
        {
            SelectedRole = Qt::UserRole + 1,
            HeaderNameRole
        };

        /*!
         * \brief rowCount Returns the row count.
         * \param parent The parent-
         * \return The row count.
         */
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        /*!
         * \brief data Returns the data for the model.
         * \param index The index for the data.
         * \param role The role.
         * \return The data for the model.
         */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        /*!
         * \brief setData Sets the data in the model.
         * \param index The index for the data to set.
         * \param value The value to set.
         * \param role The role.
         * \return True if data has changed, false otherwise.
         */
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

        /*!
         * \brief flags \c Qt::ItemFlags for this \c QAbstractListModel.
         * \param index The index.
         * \return \c Qt::ItemFlags for this \c QAbstractListModel.
         */
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        /*!
         * \brief roleNames Returns the role names of this \c QAbstractListModel inside a \c QHash<int, QByteArray>.
         * \return The role names of this \c QAbstractListModel inside a \c QHash<int, QByteArray>.
         */
        virtual QHash<int, QByteArray> roleNames() const override;

        /*!
         * \brief jobTable Returns the \c JobTable.
         * This is used to retrieve the Header Names.
         * \return The \c JobTable.
         */
        JobTable* jobTable() const;

        /*!
         * \brief selectedHeaders Returns a list containg the states of the headers.
         * True means selected, false means unselected.
         * \return
         */
        const QList<bool>& selectedHeaders() const;

        /*!
         * \brief selectedHeaderIndices Returns the indices of the selected headers.
         * \return The indices of the selected headers.
         */
        const QList<int>& selectedHeaderIndices() const;

    signals:
        /*!
         * \brief jobTableChanged Called when the \c JobTable changed.
         * \param table The new \c JobTable.
         */
        void jobTableChanged(JobTable* table);

        /*!
         * \brief selectedHeadersChanged Called when the selected headers changed.
         * \param headers the new states of the headers.
         */
        void selectedHeadersChanged(const QList<bool>& headers);

        /*!
         * \brief selectedHeaderIndicesChanged Called when the indices changed.
         * \param indices New indices.
         */
        void selectedHeaderIndicesChanged(const QList<int>& indices);

        /*!
         * \brief selectedHeader Called when the selection changed by this component.
         * \param index the index of the changed value. May be unused.
         */
        void selectedHeader(int index);

    public slots:
        /*!
         * \brief setJobTable Sets the \c JobTable.
         * \param table The new \c JobTable.
         */
        void setJobTable(JobTable* table);

        /*!
         * \brief setSelectedHeaders Sets the selected headers.
         * \param headers List of states of the headers.
         */
        void setSelectedHeaders(const QList<bool>& headers);

        /*!
         * \brief setSelectedHeaderIndices Sets the indices.
         * \param indices New indices.
         */
        void setSelectedHeaderIndices(const QList<int>& indices);

    private:
        /*!
         * \brief updateSelectedHeaderIndices Updates the list of selected header indices.
         * Lets suppose we have the states of selected headers:
         * [false, true, false, false, true]
         *
         * This function would update m_selectedHeaderIndices in a way that this list would contain:
         * [1, 4]
         */
        void updateSelectedHeaderIndices();

    private:
        /*!
         * \brief m_table The \c JobTable of this model.
         * Only the header names of the data here are used.
         * It would be also possible to store these in a seperate array owned by this object.
         * However, storing just a pointer to the already existing data is more memory friendly.
         */
        JobTable* m_table;

        /*!
         * \brief m_selectedHeaders stores the selected states of the headers.
         * True means that a header name has been selected, false means that it is not selected.
         */
        QList<bool> m_selectedHeaders;

        /*!
         * \brief m_selectedHeaderIndices Holds the indices of the selcted headers.
         */
        QList<int> m_selectedHeaderIndices;
    };
}

#endif // ARRIVAL_HEADERLISTMODEL_H
