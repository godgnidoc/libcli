import { Application } from "./application";
import { CliArg } from "./argument";
import { Feature } from "./feature";
/**
 * 执行应用
 * - 指令结构 `app [global-options] [[module...] feature] [options] [args]`
 * - 选项和功能参数的顺序不影响解析结果，但选项的参数必须紧跟在选项后面
 * - 若功能处于模块中，则模块名必须在功能名之前，功能选项必须出现在功能路径之后
 * - 短选项均由一个连字符 `-` 开头，且仅接受一个字母，短选项允许合并
 * - 长选项至少由两个连字符 `--` 开头，且至少接受一个字母
 * - 带参数的短选项若参与合并，必须放在合并后的最后一个短选项后面
 * @param app 应用描述符
 * @param args 命令行参数，应当仅包含参数，不包含命令本身
 * @return 返回值为 0 表示正常退出，否则表示异常退出
 */
export declare function execute(app: Application, args: string[]): Promise<number>;
export declare function consumeGlobalOptions(app: Application, args: CliArg[]): Promise<boolean>;
export declare function consumeFeatureOptions(feature: Feature, args: CliArg[]): Promise<boolean>;
export declare function locateGlobalElement(app: Application, args: CliArg[]): boolean;
export declare function invokeFeature(feature: Feature, args: CliArg[]): Promise<number>;
