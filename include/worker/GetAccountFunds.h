/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_GETACCOUNTFUNDS_H
#define WORKER_GETACCOUNTFUNDS_H

#include <greentop/ExchangeApi.h>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(GET_ACCOUNT_FUNDS, wxThreadEvent);

/**
 * Get account balance for a wallet.
 */
class GetAccountFunds : public Worker {
    public:

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         */
        GetAccountFunds(wxEvtHandler* eventHandler);

    protected:
        virtual ExitCode Entry();
    private:

        greentop::AccountFundsResponse DoGetAccountFunds();
};

}
}

#endif // WORKER_GETACCOUNTFUNDS_H
