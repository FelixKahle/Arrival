// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

#ifndef ARRIVAL_JOBTABLEROW_H
#define ARRIVAL_JOBTABLEROW_H

#include <QObject>
#include <QList>
#include <QString>

namespace Arrival::App
{
    /*!
     * \brief The JobTableRowState class is only used to expose the \c State enum to QML.
     */
    class JobTableRowState : public QObject
    {
        Q_OBJECT
    public:

        enum State
        {
            Added = 0,
            Removed = 1,
            Remained = 2,
            Invalid = -1
        };
        Q_ENUM(JobTableRowState::State)
    };

    struct JobTableRow
    {
        JobTableRowState::State state;
        QList<QString> columns;
    };


}

#endif // ARRIVAL_JOBTABLEROW_H
