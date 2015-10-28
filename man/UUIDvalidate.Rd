\encoding{UTF-8}
\name{UUIDvalidate}
\alias{UUIDvalidate}
\title{
  Validate UUIDs
}
\description{
  \code{UUIDvalidate} validates Universally Unique Identifier strings.
}
\usage{
UUIDvalidate(uuid)
}
\arguments{
  \item{uuid}{character vector of UUID strings.}
}
\details{
  Rather than using regular expressions, this function uses \code{libuuid}'s
  \code{uuid_parse} function for efficiency (and provides a ~5X speed
  boost vs \code{grepl}).

  NA strings in `uuid` lead to NAs in the output vector.
}
\value{
  Logical vector, possibly containing NA values (see details).
}
%\references{
%}
\author{
  Murat Taşan, using libuuid by Theodore Ts'o.
}
%\note{
%}
\examples{
uuids <- sapply(1:10, UUIDgenerate)
uuids[3] <- NA
uuids[6] <- "foo"
UUIDvalidate(uuids)
UUIDvalidate(toupper(uuids)) ## works with upper-case, too.

uuids <- sapply(1:1e5, UUIDgenerate) ## generate 100,000 UUIDs for speed test
system.time(UUIDvalidate(uuids)) ## ~0.047s on a test laptop
system.time(grepl("[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}", uuids)) ## ~0.27s on same test laptop
}
