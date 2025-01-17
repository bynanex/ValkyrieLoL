#include "ValkyrieAPI.h"
#include <thread>

ValkyrieAPI* ValkyrieAPI::Instance = nullptr;
std::mutex   LambdaInvokeResultAsync::RequestMutex;

ValkyrieAPI::ValkyrieAPI()
{
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId("AKIAU6GDVTT2BO5OFOE5");
	credentials.SetAWSSecretKey("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");

	Aws::Client::ClientConfiguration config;
	config.region = Aws::String("eu-north-1");

	lambdaClient = Aws::MakeShared<Aws::Lambda::LambdaClient>("lambda_client_tag", credentials, config);
	s3Client     = Aws::MakeShared<Aws::S3::S3Client>("s3_client_tag", credentials, config);

	lambdaInvokeRequest.SetFunctionName("valkyrie-api");
	lambdaInvokeRequest.SetInvocationType(Aws::Lambda::Model::InvocationType::RequestResponse);
	lambdaInvokeRequest.SetLogType(Aws::Lambda::Model::LogType::Tail);
	lambdaInvokeRequest.SetContentType("application/javascript");
}

std::shared_ptr<UserResultAsync> ValkyrieAPI::CreateAccount(const char * name, const char * pass, const char * discord, const HardwareInfo & hardware, const char * inviteCode)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("name", name);
	jsonParams.WithString("pass", pass);
	jsonParams.WithString("discord", discord);
	jsonParams.WithString("code", inviteCode);
	jsonParams.WithObject("hardware", hardware.ToJsonValue());

	PutOperation("create-account", jsonParams);

	return std::shared_ptr<UserResultAsync>(new UserResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<GetUserListAsync> ValkyrieAPI::GetUsers(const IdentityInfo & identity)
{
	Aws::Utils::Json::JsonValue jsonParams;
	PutIdentity(jsonParams, identity);
	PutOperation("list-users", jsonParams);

	return std::shared_ptr<GetUserListAsync>(new GetUserListAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<UserResultAsync> ValkyrieAPI::GetUser(const IdentityInfo & identity, const char* target)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("target", target);
	PutIdentity(jsonParams, identity);
	PutOperation("get-user", jsonParams);

	return std::shared_ptr<UserResultAsync>(new UserResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<InviteListResultAsync> ValkyrieAPI::GenerateInviteCodes(const IdentityInfo & identity, const InviteInfo& invite, int numToGenerate)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithObject("invite", invite.ToJsonValue());
	jsonParams.WithInteger("num-invites", numToGenerate);

	PutIdentity(jsonParams, identity);
	PutOperation("generate-invite", jsonParams);

	return std::shared_ptr<InviteListResultAsync>(new InviteListResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<UserResultAsync> ValkyrieAPI::UpdateUser(const IdentityInfo & identity, const char * target, const UserInfo & targetInfo)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("target", target);
	jsonParams.WithObject("target-info", targetInfo.ToJsonValue());
	PutIdentity(jsonParams, identity);
	PutOperation("update-user", jsonParams);

	return std::shared_ptr<UserResultAsync>(new UserResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<ScriptListAsync> ValkyrieAPI::GetScriptList(const IdentityInfo & identity)
{
	Aws::Utils::Json::JsonValue jsonParams;
	PutIdentity(jsonParams, identity);
	PutOperation("list-scripts", jsonParams);

	return std::shared_ptr<ScriptListAsync>(new ScriptListAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<StringResultAsync> ValkyrieAPI::GetScriptCode(const IdentityInfo & identity, std::string& id)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("id", id.c_str());
	PutIdentity(jsonParams, identity);
	PutOperation("get-script-code", jsonParams);

	return std::shared_ptr<StringResultAsync>(new StringResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<ScriptSubmissionsResultAsync> ValkyrieAPI::SubmitScript(const IdentityInfo & identity, const ScriptInfo & script, const std::string & code)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithObject("script", script.ToJsonValue());
	jsonParams.WithString("code", code.c_str());
	PutIdentity(jsonParams, identity);
	PutOperation("submit-script", jsonParams);

	return std::shared_ptr<ScriptSubmissionsResultAsync>(new ScriptSubmissionsResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<ScriptSubmissionsResultAsync> ValkyrieAPI::GetSubmissions(const IdentityInfo & identity, const std::string & name)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("target", name.c_str());
	PutIdentity(jsonParams, identity);
	PutOperation("submissions-for", jsonParams);

	return std::shared_ptr<ScriptSubmissionsResultAsync>(new ScriptSubmissionsResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<ScriptSubmissionsResultAsync> ValkyrieAPI::GetAllSubmissions(const IdentityInfo & identity)
{
	Aws::Utils::Json::JsonValue jsonParams;
	PutIdentity(jsonParams, identity);
	PutOperation("list-submissions", jsonParams);

	return std::shared_ptr<ScriptSubmissionsResultAsync>(new ScriptSubmissionsResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<ScriptRatingResultAsync> ValkyrieAPI::RateScript(const IdentityInfo & identity, const std::string & scriptId, float rating)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("id", scriptId.c_str());
	jsonParams.WithDouble("rating", (double)rating);

	PutIdentity(jsonParams, identity);
	PutOperation("rate-script", jsonParams);

	return std::shared_ptr<ScriptRatingResultAsync>(new ScriptRatingResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<LambdaInvokeResultAsync> ValkyrieAPI::UpdateSubmission(const IdentityInfo & identity, const ScriptSubmission & submission)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithObject("script-submission", submission.ToJsonValue());
	PutIdentity(jsonParams, identity);
	PutOperation("update-submission", jsonParams);

	return std::shared_ptr<ScriptSubmissionsResultAsync>(new ScriptSubmissionsResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<StringResultAsync> ValkyrieAPI::ExtendSubscription(const char * name, const char * code)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithString("name", name);
	jsonParams.WithString("code", code);
	PutOperation("extend-sub", jsonParams);

	return std::shared_ptr<StringResultAsync>(new StringResultAsync(*lambdaClient, lambdaInvokeRequest));
}

std::shared_ptr<LambdaInvokeResultAsync> ValkyrieAPI::LogSession(const IdentityInfo & identity, const SessionInfo & session)
{
	Aws::Utils::Json::JsonValue jsonParams;
	jsonParams.WithObject("session-info", session.ToJsonValue());
	PutIdentity(jsonParams, identity);
	PutOperation("log-session", jsonParams);

	auto task = std::shared_ptr<ScriptSubmissionsResultAsync>(new ScriptSubmissionsResultAsync(*lambdaClient, lambdaInvokeRequest));
	task->hidden = true;

	return task;
}

ValkyrieAPI* ValkyrieAPI::Get()
{
	if (Instance == nullptr)
		Instance = new ValkyrieAPI();

	return Instance;
}

void ValkyrieAPI::PutIdentity(JsonValue & json, const IdentityInfo & identity)
{
	json.WithString("name", identity.name.c_str());
	json.WithString("pass", identity.pass.c_str());
	json.WithObject("hardware", identity.hardware.ToJsonValue());
}

void ValkyrieAPI::PutOperation(const char* operation, JsonValue& params)
{
	std::shared_ptr<Aws::IOStream> payload = Aws::MakeShared<Aws::StringStream>(operation);

	Aws::Utils::Json::JsonValue json;
	json.WithString("operation", operation);
	json.WithObject("operation-params", params);

	*payload << json.View().WriteReadable();
	lambdaInvokeRequest.SetBody(payload);
}

std::shared_ptr<GetS3ObjectHeadResultAsync> ValkyrieAPI::GetS3ObjectHead(const char * bucket, const char * key)
{
	s3HeadObjectRequest.SetBucket(bucket);
	s3HeadObjectRequest.SetKey(key);
	return std::shared_ptr<GetS3ObjectHeadResultAsync>(new GetS3ObjectHeadResultAsync(*s3Client, s3HeadObjectRequest));
}

std::shared_ptr<GetS3ObjectAsync> ValkyrieAPI::GetCheatS3Object(const char* bucket, const char* key)
{
	s3GetObjectRequest.SetBucket(bucket);
	s3GetObjectRequest.SetKey(key);

	return std::shared_ptr<GetS3ObjectAsync>(new GetS3ObjectAsync(*s3Client, s3GetObjectRequest));
}