#pragma once

#include <Common/config.h>

#if USE_AWS_S3

#include <Common/Exception.h>
#include <common/types.h>

#include <aws/core/Aws.h>  // Y_IGNORE
#include <aws/core/client/ClientConfiguration.h>  // Y_IGNORE
#include <aws/s3/S3Client.h>  // Y_IGNORE
#include <aws/s3/model/HeadObjectRequest.h>  // Y_IGNORE

#include <IO/S3/PocoHTTPClient.h>
#include <Poco/URI.h>


namespace Aws::S3
{
    class S3Client;
}

namespace DB
{
    class RemoteHostFilter;
    struct HttpHeader;
    using HeaderCollection = std::vector<HttpHeader>;

    namespace ErrorCodes
    {
        extern const int S3_ERROR;
    }

}

namespace DB::S3
{
class ClientFactory
{
public:
    ~ClientFactory();

    static ClientFactory & instance();

    std::shared_ptr<Aws::S3::S3Client> create(
        const PocoHTTPClientConfiguration & cfg,
        bool is_virtual_hosted_style,
        const String & access_key_id,
        const String & secret_access_key,
        const String & server_side_encryption_customer_key_base64,
        HeaderCollection headers,
        bool use_environment_credentials,
        bool use_insecure_imds_request);

    PocoHTTPClientConfiguration createClientConfiguration(
        const String & force_region,
        const RemoteHostFilter & remote_host_filter,
        unsigned int s3_max_redirects);

private:
    ClientFactory();

private:
    Aws::SDKOptions aws_options;
};

/**
 * Represents S3 URI.
 *
 * The following patterns are allowed:
 * s3://bucket/key
 * http(s)://endpoint/bucket/key
 */
struct URI
{
    Poco::URI uri;
    // Custom endpoint if URI scheme is not S3.
    String endpoint;
    String bucket;
    String key;
    String storage_name;

    bool is_virtual_hosted_style;

    explicit URI(const Poco::URI & uri_);
};

size_t getObjectSize(std::shared_ptr<Aws::S3::S3Client> client_ptr, const String & bucket, const String & key, bool throw_on_error = true);

}

#endif
