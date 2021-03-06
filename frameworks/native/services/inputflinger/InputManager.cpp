#include "InputManager.h"
#include "dispatcher/include/InputDispatcherFactory.h"
#include "reader/include/InputReaderFactory.h"
#include "dispatcher/include/InputDispatcherThread.h"

namespace android {
    InputManager::InputManager(const android::sp<android::InputReaderPolicyInterface> &readerPolicy,
                               const android::sp<android::InputDispatcherPolicyInterface> &dispatcherPolicy) {
        mDispatcher = createInputDispatcher(dispatcherPolicy);
        mClassifier = new InputClassifier(mDispatcher);
        mReader = createInputReader(readerPolicy, mClassifier);
        initialize();
    }

    status_t InputManager::start() {
        status_t result = mDispatcherThread->run("InputDispatcher", PRIORITY_URGENT_DISPLAY);
        // fixme

        result = mReaderThread->run("InputReader", PRIORITY_URGENT_DISPLAY);

        return OK;
    }

    void InputManager::initialize() {
        mReaderThread = new InputReaderThread(mReader);
        mDispatcherThread = new InputDispatcherThread(mDispatcher);
    }

    sp<InputDispatcherInterface> InputManager::getDispatcher() {
        return mDispatcher;
    }
};