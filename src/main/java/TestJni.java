import com.zenvv.capi.*;

public class TestJni {
    public static void main(String[] args) {
        byte[] encoded = null;

        {
            byte[] data = new byte[960];
            AEncoder enc = new AEncoder(AEncoder.OPUS_CODEC);
            encoded = enc.Encode(data);
            int err = enc.GetLastError();
            enc.Close();
            int len = (encoded != null) ? encoded.length : 0;
            System.out.println("encode:" + len + ", err:" + err);
        }

        {
            ADecoder dec = new ADecoder(ADecoder.OPUS_CODEC);
            byte[] ret = dec.Decode(encoded);
            int err = dec.GetLastError();
            dec.Close();
            int len = (ret != null) ? ret.length : 0;
            System.out.println("decode1:" + len + ", err:" + err);
        }

        {
            ADecoder dec = new ADecoder(ADecoder.OPUS_CODEC);
            short[] ret = dec.DecodeEx(encoded);
            int err = dec.GetLastError();
            dec.Close();
            int len = (ret != null) ? ret.length : 0;
            System.out.println("decode2:" + len + ", err:" + err);

            Resampler re = new Resampler(32000, 16000);
            short[] ret2 = re.Push(ret);
            re.Close();
            int len2 = (ret2 != null) ? ret2.length : 0;
            System.out.println("resample2:" + len2);
        }
    }
}
