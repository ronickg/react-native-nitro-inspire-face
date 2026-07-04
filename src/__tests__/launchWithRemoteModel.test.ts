// Import the mock helpers by path (not the package name) so this also
// type-checks — the real 'react-native-nitro-modules' has no __getHybrid. Under
// jest this resolves to the SAME module the auto-mock uses for index.tsx, so
// they share stub state. Imported once: index.tsx captures ModelManager/
// InspireFace at load and the mock returns stable stub instances, so these
// resolve to the SAME objects the flow uses. (Do NOT jest.resetModules().)
import {
  __getHybrid,
  __resetHybrids,
} from '../../__mocks__/react-native-nitro-modules';
import { launchWithRemoteModel } from '../index';

const CHECKSUM =
  '5037ba1f49905b783a1c973d5d58b834a645922cc2814c8e3ca630a38dc24431';
const URL = 'https://example.com/Pikachu';
const model = __getHybrid<{
  getCachedModel: jest.Mock;
  downloadModel: jest.Mock;
}>('ModelManager');
const inspire = __getHybrid<{ launch: jest.Mock }>('InspireFace');

beforeAll(() => jest.spyOn(console, 'log').mockImplementation(() => {}));

describe('launchWithRemoteModel', () => {
  beforeEach(() => __resetHybrids());

  it('uses the cached model and does NOT download when the cache hits', async () => {
    model.getCachedModel.mockReturnValue('/cache/Pikachu');

    await launchWithRemoteModel(URL, CHECKSUM);

    expect(model.getCachedModel).toHaveBeenCalledWith(CHECKSUM);
    expect(model.downloadModel).not.toHaveBeenCalled();
    expect(inspire.launch).toHaveBeenCalledWith('/cache/Pikachu');
  });

  it('downloads then launches on a cache miss', async () => {
    model.getCachedModel.mockReturnValue(null);
    model.downloadModel.mockResolvedValue('/dl/Pikachu');

    await launchWithRemoteModel(URL, CHECKSUM);

    expect(model.downloadModel).toHaveBeenCalledWith(URL, CHECKSUM);
    expect(inspire.launch).toHaveBeenCalledWith('/dl/Pikachu');
  });

  it('propagates a download failure and never launches', async () => {
    model.getCachedModel.mockReturnValue(null);
    model.downloadModel.mockRejectedValue(new Error('checksum mismatch'));

    await expect(launchWithRemoteModel(URL, CHECKSUM)).rejects.toThrow(
      'checksum mismatch'
    );
    expect(inspire.launch).not.toHaveBeenCalled();
  });
});
