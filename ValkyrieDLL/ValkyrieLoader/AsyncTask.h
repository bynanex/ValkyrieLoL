#pragma once
#include <string>

enum AsyncTaskStatus {
	ASYNC_NOT_STARTED,
	ASYNC_RUNNING,
	ASYNC_SUCCEEDED,
	ASYNC_FAILED
};

class AsyncTask {

public:
	float           percentDone = -1;
	std::string     currentStep;
	std::string     error;
	std::string     taskId;

	std::function<void(std::shared_ptr<AsyncTask>)> onSuccess;

	virtual void Perform() = 0;

	void SetStatus(AsyncTaskStatus s) {
		status = s;
	}

	AsyncTaskStatus GetStatus() {
		return status;
	}
private:
	AsyncTaskStatus status;
};